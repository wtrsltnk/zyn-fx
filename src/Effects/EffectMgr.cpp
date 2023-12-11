/*
  ZynAddSubFX - a software synthesizer

  EffectMgr.cpp - Effect manager, an interface betwen the program and effects
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

#include "EffectMgr.h"

#include "../Misc/Util.h"

#include "Effect.h"

#ifdef ZYN_ALIENWAH_VST
#include "Alienwah.h"
#endif
#ifdef ZYN_CHORUS_VST
#include "Chorus.h"
#endif
#ifdef ZYN_DISTORSION_VST
#include "Distorsion.h"
#endif
#ifdef ZYN_DYNAMICFILTER_VST
#include "DynamicFilter.h"
#endif
#ifdef ZYN_EQ_VST
#include "EQ.h"
#endif
#ifdef ZYN_ECHO_VST
#include "Echo.h"
#endif
#ifdef ZYN_PHASER_VST
#include "Phaser.h"
#endif
#ifdef ZYN_REVERB_VST
#include "Reverb.h"
#endif

EffectMgr::EffectMgr(
    std::mutex *mutex_)
    : mutex(mutex_)
{
    efxoutl = new float[synth->buffersize];
    efxoutr = new float[synth->buffersize];
    memset(efxoutl, 0, synth->bufferbytes);
    memset(efxoutr, 0, synth->bufferbytes);
    defaults();
}

EffectMgr::~EffectMgr()
{
    delete efx;
    delete[] efxoutl;
    delete[] efxoutr;
}

void EffectMgr::defaults()
{
    changeeffect(EffectTypes::None);

    setdryonly(false);
}

// Change the effect
void EffectMgr::changeeffect(
    EffectTypes _nefx)
{
    cleanup();

    if (nefx == _nefx)
    {
        return;
    }

    nefx = _nefx;
    memset(efxoutl, 0, synth->bufferbytes);
    memset(efxoutr, 0, synth->bufferbytes);
    delete efx;
    switch (nefx)
    {
#ifdef ZYN_REVERB_VST
        case EffectTypes::ReverbType:
            efx = new Reverb(efxoutl, efxoutr, synth->samplerate, synth->buffersize);
            break;
#endif
#ifdef ZYN_ECHO_VST
        case EffectTypes::EchoType:
            efx = new Echo(efxoutl, efxoutr, synth->samplerate, synth->buffersize);
            break;
#endif
#ifdef ZYN_CHORUS_VST
        case EffectTypes::ChorusType:
            efx = new Chorus(efxoutl, efxoutr, synth->samplerate, synth->buffersize);
            break;
#endif
#ifdef ZYN_PHASER_VST
        case EffectTypes::PhaserType:
            efx = new Phaser(efxoutl, efxoutr, synth->samplerate, synth->buffersize);
            break;
#endif
#ifdef ZYN_ALIENWAH_VST
        case EffectTypes::AlienwahType:
            efx = new Alienwah(efxoutl, efxoutr, synth->samplerate, synth->buffersize);
            break;
#endif
#ifdef ZYN_DISTORSION_VST
        case EffectTypes::DistorsionType:
            efx = new Distorsion(efxoutl, efxoutr, synth->samplerate, synth->buffersize);
            break;
#endif
#ifdef ZYN_EQ_VST
        case EffectTypes::EQType:
            efx = new EQ(efxoutl, efxoutr, synth->samplerate, synth->buffersize);
            break;
#endif
#ifdef ZYN_DYNAMICFILTER_VST
        case EffectTypes::DynamicFilterType:
            efx = new DynamicFilter(efxoutl, efxoutr, synth->samplerate, synth->buffersize);
            break;
#endif
        // put more effect here
        default:
            efx = nullptr;
            break; // no effect (thru)
    }

    if (efx)
    {
        filterpars = efx->filterpars;
    }
}

// Obtain the effect number
EffectTypes EffectMgr::geteffect()
{
    return nefx;
}

// Cleanup the current effect
void EffectMgr::cleanup()
{
    if (efx)
    {
        efx->cleanup();
    }
}

// Get the preset of the current effect
unsigned char EffectMgr::getpreset()
{
    if (efx)
    {
        return efx->Ppreset;
    }

    return 0;
}

// Change the preset of the current effect
void EffectMgr::changepreset_nolock(
    unsigned char npreset)
{
    if (efx)
    {
        efx->setpreset(npreset);
    }
}

// Change the preset of the current effect(with thread locking)
void EffectMgr::changepreset(
    unsigned char npreset)
{
    mutex->lock();
    changepreset_nolock(npreset);
    mutex->unlock();
}

// Change a parameter of the current effect
void EffectMgr::seteffectpar_nolock(
    int npar,
    unsigned char value)
{
    if (!efx)
    {
        return;
    }

    efx->changepar(npar, value);
}

// Change a parameter of the current effect (with thread locking)
void EffectMgr::seteffectpar(
    int npar,
    unsigned char value)
{
    mutex->lock();
    seteffectpar_nolock(npar, value);
    mutex->unlock();
}

// Get a parameter of the current effect
unsigned char EffectMgr::geteffectpar(
    int npar)
{
    if (!efx)
    {
        return 0;
    }

    return efx->getpar(npar);
}

// Apply the effect
int EffectMgr::out(
    float *smpsl,
    float *smpsr,
    int bufferSize)
{
    auto bufSize = std::min(bufferSize, synth->buffersize);

    if (!efx)
    {
        return bufSize;
    }

    for (int i = 0; i < bufSize; ++i)
    {
        smpsl[i] += denormalkillbuf[i];
        smpsr[i] += denormalkillbuf[i];
        efxoutl[i] = 0.0f;
        efxoutr[i] = 0.0f;
    }

    efx->out(smpsl, smpsr, bufSize);

    float volume = efx->volume;

#ifdef ZYN_EQ_VST
    if (nefx == EffectTypes::EQType)
    {
        // this is need only for the EQ effect
        memcpy(smpsl, efxoutl, synth->bufferbytes);
        memcpy(smpsr, efxoutr, synth->bufferbytes);

        return bufferSize;
    }
#endif

    // System effect
    for (int i = 0; i < bufSize; ++i)
    {
        efxoutl[i] *= 2.0f * volume;
        efxoutr[i] *= 2.0f * volume;
        smpsl[i] = efxoutl[i];
        smpsr[i] = efxoutr[i];
    }

    return bufSize;
}

// Get the effect volume for the system effect
float EffectMgr::sysefxgetvolume()
{
    return (!efx) ? 1.0f : efx->outvolume;
}

// Get the EQ response
float EffectMgr::getEQfreqresponse(
    float freq)
{
    return (nefx == EffectTypes::EQType) ? efx->getfreqresponse(freq) : 0.0f;
}

void EffectMgr::setdryonly(
    bool value)
{
    dryonly = value;
}
