#include "zyn.chorus.h"

#include <string.h>

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

//-------------------------------------------------------------------------------------------------------
ZynAudioEffectX::ZynAudioEffectX(audioMasterCallback audioMaster)
    : AudioEffectX(audioMaster, 1, 1) // 1 program, 1 parameter only
{
    setNumInputs(2);       // stereo in
    setNumOutputs(2);      // stereo out
    setUniqueID('ZCHO');   // identify
    canProcessReplacing(); // supports replacing output

    _gain = 1.f;                                           // default to 0 dB
    strcpy_s(_programName, kVstMaxProgNameLen, "Default"); // default program name

    _effectMgr = std::make_unique<EffectMgr>(&_mutex);

    _effectMgr->changeeffect(EffectTypes::ChorusType);
}

//-------------------------------------------------------------------------------------------------------
ZynAudioEffectX::~ZynAudioEffectX()
{
    // nothing to do here
}

//-----------------------------------------------------------------------------------------
void ZynAudioEffectX::processReplacing(
    float **inputs,
    float **outputs,
    VstInt32 sampleFrames)
{
    float *in1 = inputs[0];
    float *in2 = inputs[1];

    float *out1 = outputs[0];
    float *out2 = outputs[1];

    do
    {
        auto frames = _effectMgr->out(in1, in2, sampleFrames);

        float *procl = _effectMgr->efxoutl;
        float *procr = _effectMgr->efxoutr;

        for (int i = 0; i < frames; i++)
        {
            (*out1++) = (*procl++) * _gain;
            (*out2++) = (*procr++) * _gain;
        }

        sampleFrames -= frames;

    } while (sampleFrames > 0);
}

//-------------------------------------------------------------------------------------------------------
void ZynAudioEffectX::setProgramName(char *name)
{
    strcpy_s(_programName, kVstMaxProgNameLen, name);
}

//-----------------------------------------------------------------------------------------
void ZynAudioEffectX::getProgramName(char *name)
{
    strcpy_s(name, kVstMaxProgNameLen, _programName);
}

//-----------------------------------------------------------------------------------------
void ZynAudioEffectX::setParameter(VstInt32 index, float value)
{
    _gain = value;
}

//-----------------------------------------------------------------------------------------
float ZynAudioEffectX::getParameter(VstInt32 index)
{
    return _gain;
}

//-----------------------------------------------------------------------------------------
void ZynAudioEffectX::getParameterName(VstInt32 index, char *label)
{
    strcpy_s(label, kVstMaxParamStrLen, "Gain");
}

//-----------------------------------------------------------------------------------------
void ZynAudioEffectX::getParameterDisplay(VstInt32 index, char *text)
{
    dB2string(_gain, text, kVstMaxParamStrLen);
}

//-----------------------------------------------------------------------------------------
void ZynAudioEffectX::getParameterLabel(VstInt32 index, char *label)
{
    strcpy_s(label, kVstMaxParamStrLen, "dB");
}

//------------------------------------------------------------------------
bool ZynAudioEffectX::getEffectName(char *name)
{
    strcpy_s(name, kVstMaxEffectNameLen, "Zyn.Chorus");
    return true;
}

//------------------------------------------------------------------------
bool ZynAudioEffectX::getProductString(char *text)
{
    strcpy_s(text, kVstMaxProductStrLen, "Zyn.Chorus");
    return true;
}

//------------------------------------------------------------------------
bool ZynAudioEffectX::getVendorString(char *text)
{
    strcpy_s(text, kVstMaxVendorStrLen, "ZynAddSubFx");
    return true;
}

//-----------------------------------------------------------------------------------------
VstInt32 ZynAudioEffectX::getVendorVersion()
{
    return 1001;
}
