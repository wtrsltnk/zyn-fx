/*
  ZynAddSubFX - a software synthesizer

  Alienwah.cpp - "AlienWah" effect
  Copyright (C) 2002-2005 Nasca Octavian Paul
  Author: Nasca Octavian Paul

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License
  as published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License (version 2 or later) for more details.

  You should have received a copy of the GNU General Public License (version 2)
  along with this program; if not, write to the Free Software Foundation,
  Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

*/

#include "Alienwah.h"
#include "EffectPresets.h"
#include <cmath>

Alienwah::Alienwah(
    float *efxoutl_,
    float *efxoutr_,
    unsigned int srate,
    int bufsize)
    : Effect(efxoutl_, efxoutr_, NULL, 0, srate, bufsize),
      lfo(srate, bufsize),
      oldl(NULL),
      oldr(NULL)
{
    setpreset(Ppreset);
    cleanup();
    oldclfol = complex<float>(fb, 0.0f);
    oldclfor = complex<float>(fb, 0.0f);
}

Alienwah::~Alienwah()
{
    if (oldl != NULL)
        delete[] oldl;
    if (oldr != NULL)
        delete[] oldr;
}

// Apply the effect
void Alienwah::out(const Stereo<float *> &smp,
                   int bufSize)
{
    buffersize = bufSize;

    float lfol, lfor; // Left/Right LFOs
    complex<float> clfol, clfor;
    /**\todo Rework, as optimization can be used when the new complex type is
     * utilized.
     * Before all calculations needed to be done with individual float,
     * but now they can be done together*/
    lfo.effectlfoout(&lfol, &lfor);
    lfol *= depth * PI * 2.0f;
    lfor *= depth * PI * 2.0f;
    clfol = complex<float>(cosf(lfol + phase) * fb, sinf(lfol + phase) * fb); // rework
    clfor = complex<float>(cosf(lfor + phase) * fb, sinf(lfor + phase) * fb); // rework

    for (int i = 0; i < buffersize; ++i)
    {
        float x = ((float)i) / buffersize_f;
        float x1 = 1.0f - x;
        // left
        complex<float> tmp = clfol * x + oldclfol * x1;

        complex<float> out = tmp * oldl[oldk];
        out += (1 - fabs(fb)) * smp.l[i] * pangainL;

        oldl[oldk] = out;
        float l = out.real() * 10.0f * (fb + 0.1f);

        // right
        tmp = clfor * x + oldclfor * x1;

        out = tmp * oldr[oldk];
        out += (1 - fabs(fb)) * smp.r[i] * pangainR;

        oldr[oldk] = out;
        float r = out.real() * 10.0f * (fb + 0.1f);

        if (++oldk >= Pdelay)
            oldk = 0;
        // LRcross
        efxoutl[i] = l * (1.0f - lrcross) + r * lrcross;
        efxoutr[i] = r * (1.0f - lrcross) + l * lrcross;
    }

    oldclfol = clfol;
    oldclfor = clfor;
}

// Cleanup the effect
void Alienwah::cleanup(void)
{
    for (int i = 0; i < Pdelay; ++i)
    {
        oldl[i] = complex<float>(0.0f, 0.0f);
        oldr[i] = complex<float>(0.0f, 0.0f);
    }
    oldk = 0;
}

// Parameter control
void Alienwah::setdepth(unsigned char _Pdepth)
{
    Pdepth = _Pdepth;
    depth = Pdepth / 127.0f;
}

void Alienwah::setfb(unsigned char _Pfb)
{
    Pfb = _Pfb;
    fb = fabs((Pfb - 64.0f) / 64.1f);
    fb = sqrtf(fb);
    if (fb < 0.4f)
        fb = 0.4f;
    if (Pfb < 64)
        fb = -fb;
}

void Alienwah::setvolume(unsigned char _Pvolume)
{
    Pvolume = _Pvolume;
    outvolume = Pvolume / 127.0f;
    volume = 1.0f;
}

void Alienwah::setphase(unsigned char _Pphase)
{
    Pphase = _Pphase;
    phase = (Pphase - 64.0f) / 64.0f * PI;
}

void Alienwah::setdelay(unsigned char _Pdelay)
{
    if (oldl != NULL)
        delete[] oldl;
    if (oldr != NULL)
        delete[] oldr;
    Pdelay = (_Pdelay >= MAX_ALIENWAH_DELAY) ? MAX_ALIENWAH_DELAY : _Pdelay;
    oldl = new complex<float>[Pdelay];
    oldr = new complex<float>[Pdelay];
    cleanup();
}

void Alienwah::setpreset(unsigned char npreset)
{
    unsigned char presets[ALIENWAH_NUM_PRESETS][ALIENWAH_PRESET_SIZE] = {
        // AlienWah1
        {127, 64, 70, 0, 0, 62, 60, 105, 25, 0, 64},
        // AlienWah2
        {127, 64, 73, 106, 0, 101, 60, 105, 17, 0, 64},
        // AlienWah3
        {127, 64, 63, 0, 1, 100, 112, 105, 31, 0, 42},
        // AlienWah4
        {93, 64, 25, 0, 1, 66, 101, 11, 47, 0, 86}};

    if (npreset >= ALIENWAH_NUM_PRESETS)
        npreset = ALIENWAH_NUM_PRESETS - 1;
    for (int n = 0; n < ALIENWAH_PRESET_SIZE; ++n)
        changepar(n, presets[npreset][n]);

    changepar(0, presets[npreset][0] / 2); // lower the volume if this is system effect
    Ppreset = npreset;
}

void Alienwah::changepar(int npar, unsigned char value)
{
    switch (npar)
    {
        case EffectPresets::Volume:
            setvolume(value);
            break;
        case EffectPresets::Panning:
            setpanning(static_cast<char>(value));
            break;
        case EffectPresets::LFOFrequency:
            lfo.Pfreq = value;
            lfo.updateparams();
            break;
        case EffectPresets::LFORandomness:
            lfo.Prandomness = value;
            lfo.updateparams();
            break;
        case EffectPresets::LFOFunction:
            lfo.PLFOtype = value;
            lfo.updateparams();
            break;
        case EffectPresets::LFOStereo:
            lfo.Pstereo = value;
            lfo.updateparams();
            break;
        case AlienWahPresets::AlienWahDepth:
            setdepth(value);
            break;
        case AlienWahPresets::AlienWahFeedback:
            setfb(value);
            break;
        case AlienWahPresets::AlienWahDelay:
            setdelay(value);
            break;
        case AlienWahPresets::AlienWahChannelRouting:
            setlrcross(static_cast<char>(value));
            break;
        case AlienWahPresets::AlienWahPhase:
            setphase(value);
            break;
    }
}

unsigned char Alienwah::getpar(int npar) const
{
    switch (npar)
    {
        case EffectPresets::Volume:
            return Pvolume;
        case EffectPresets::Panning:
            return static_cast<unsigned char>(Ppanning);
        case EffectPresets::LFOFrequency:
            return lfo.Pfreq;
        case EffectPresets::LFORandomness:
            return lfo.Prandomness;
        case EffectPresets::LFOFunction:
            return lfo.PLFOtype;
        case EffectPresets::LFOStereo:
            return lfo.Pstereo;
        case AlienWahPresets::AlienWahDepth:
            return Pdepth;
        case AlienWahPresets::AlienWahFeedback:
            return Pfb;
        case AlienWahPresets::AlienWahDelay:
            return Pdelay;
        case AlienWahPresets::AlienWahChannelRouting:
            return static_cast<unsigned char>(Plrcross);
        case AlienWahPresets::AlienWahPhase:
            return Pphase;
        default:
            return 0;
    }
}
