#include "zynaudioeffectx.h"

#include "misc/Util.h"

static SYNTH_T s_synth;
SYNTH_T *synth = &s_synth;

//-------------------------------------------------------------------------------------------------------
AudioEffect *createEffectInstance(audioMasterCallback audioMaster)
{
    s_synth.buffersize = 480;
    s_synth.alias();

    denormalkillbuf = new float[synth->buffersize];
    for (int i = 0; i < synth->buffersize; ++i)
    {
        denormalkillbuf[i] = (RND - 0.5f) * 1e-16;
    }

    return new ZynAudioEffectX(audioMaster);
}
