/*
  ZynAddSubFX - a software synthesizer

  Reverb.h - Reverberation effect
  Copyright (C) 2002-2009 Nasca Octavian Paul
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

#ifndef REVERB_H
#define REVERB_H

#include "Effect.h"

#define REV_COMBS 8
#define REV_APS 4

/**Creates Reverberation Effects*/
class Reverb : public Effect
{
public:
    Reverb(
        float *efxoutl_,
        float *efxoutr_,
        unsigned int srate,
        int bufsize);

    ~Reverb();

    void out(
        const Stereo<float *> &smp,
        int bufferSize);

    void cleanup();

    void setpreset(
        unsigned char npreset);

    void changepar(
        int npar,
        unsigned char value);

    unsigned char getpar(
        int npar) const;

private:
    // Parametrii
    unsigned char Pvolume = 48;
    unsigned char Ptime = 64;    // duration
    unsigned char Pidelay = 40;  // initial delay
    unsigned char Pidelayfb = 0; // initial feedback
    unsigned char Plpf = 127;
    unsigned char Phpf = 0;
    unsigned char Plohidamp = 80;  // Low/HighFrequency Damping
    unsigned char Ptype = 1;       // reverb type
    unsigned char Proomsize = 64;  // room size
    unsigned char Pbandwidth = 30; // bandwidth

    // parameter control
    void setvolume(
        unsigned char _Pvolume);

    void settime(
        unsigned char _Ptime);

    void setlohidamp(
        unsigned char _Plohidamp);

    void setidelay(
        unsigned char _Pidelay);

    void setidelayfb(
        unsigned char _Pidelayfb);

    void sethpf(
        unsigned char _Phpf);

    void setlpf(
        unsigned char _Plpf);

    void settype(
        unsigned char _Ptype);

    void setroomsize(
        unsigned char _Proomsize);

    void setbandwidth(
        unsigned char _Pbandwidth);

    void processmono(
        int ch, float *output, float *inputbuf);

    // Parameters
    int lohidamptype = 0; // 0=disable, 1=highdamp (lowpass), 2=lowdamp (highpass)
    int idelaylen = 0;
    int idelayk = 0;
    float lohifb = 0.0f;
    float idelayfb = 0.0f;
    float roomsize = 1.0f;
    float rs = 1.0f; // rs is used to "normalise" the volume according to the roomsize
    int comblen[REV_COMBS * 2] = {0};
    int aplen[REV_APS * 2] = {0};
    class Unison *bandwidth = nullptr;

    // Internal Variables
    float *comb[REV_COMBS * 2] = {nullptr};
    int combk[REV_COMBS * 2] = {0};
    float combfb[REV_COMBS * 2] = {0.0f}; // feedback-ul fiecarui filtru "comb"
    float lpcomb[REV_COMBS * 2] = {0.0f}; // pentru Filtrul LowPass
    float *ap[REV_APS * 2] = {nullptr};
    int apk[REV_APS * 2] = {0};
    float *idelay = nullptr;
    class AnalogFilter *lpf = nullptr, *hpf = nullptr; // filters
};

#endif
