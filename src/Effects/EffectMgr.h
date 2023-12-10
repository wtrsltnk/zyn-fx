/*
  ZynAddSubFX - a software synthesizer

  EffectMgr.h - Effect manager, an interface betwen the program and effects
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

#ifndef EFFECTMGR_H
#define EFFECTMGR_H

#include <mutex>

class Effect;
class FilterParams;

#include "../Params/FilterParams.h"

enum EffectTypes
{
    None = 0,
    ReverbType = 1,
    EchoType = 2,
    ChorusType = 3,
    PhaserType = 4,
    AlienwahType = 5,
    DistorsionType = 6,
    EQType = 7,
    DynamicFilterType = 8,
};

/**Effect manager, an interface betwen the program and effects*/
class EffectMgr
{
public:
    EffectMgr(
        std::mutex *mutex_);

    ~EffectMgr();

    void defaults();

    int out(
        float *smpsl,
        float *smpsr,
        int bufferSize);

    void setdryonly(
        bool value);

    /**get the output(to speakers) volume of the systemeffect*/
    float sysefxgetvolume();

    void cleanup();

    void changeeffect(
        EffectTypes nefx_);

    EffectTypes geteffect();

    void changepreset(
        unsigned char npreset);

    void changepreset_nolock(
        unsigned char npreset);
    unsigned char getpreset();

    void seteffectpar(
        int npar,
        unsigned char value);

    void seteffectpar_nolock(
        int npar,
        unsigned char value);

    unsigned char geteffectpar(
        int npar);

    float *efxoutl = nullptr, *efxoutr = nullptr;

    // used by UI
    float getEQfreqresponse(
        float freq);

    FilterParams *filterpars = nullptr;

private:
    EffectTypes nefx = EffectTypes::None;
    Effect *efx = nullptr;
    std::mutex *mutex = nullptr;
    bool dryonly = false;
};

#endif
