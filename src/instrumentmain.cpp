#include "minimal.h"

#include <string.h>

//-------------------------------------------------------------------------------------------------------
AudioEffect *createEffectInstance(audioMasterCallback audioMaster)
{
    return new Minimal(audioMaster);
}

//-------------------------------------------------------------------------------------------------------
Minimal::Minimal(audioMasterCallback audioMaster)
    : AudioEffectX(audioMaster, 1, 1) // 1 program, 1 parameter only
{
    setNumInputs(2);       // stereo in
    setNumOutputs(2);      // stereo out
    setUniqueID('Gain');   // identify
    canProcessReplacing(); // supports replacing output
    canDoubleReplacing();  // supports double precision processing

    _gain = 1.f;                                           // default to 0 dB
    strcpy_s(_programName, kVstMaxProgNameLen, "Default"); // default program name
}

//-------------------------------------------------------------------------------------------------------
Minimal::~Minimal()
{
    // nothing to do here
}

//-------------------------------------------------------------------------------------------------------
void Minimal::setProgramName(char *name)
{
    strcpy_s(_programName, kVstMaxProgNameLen, name);
}

//-----------------------------------------------------------------------------------------
void Minimal::getProgramName(char *name)
{
    strcpy_s(name, kVstMaxProgNameLen, _programName);
}

//-----------------------------------------------------------------------------------------
void Minimal::setParameter(VstInt32 index, float value)
{
    _gain = value;
}

//-----------------------------------------------------------------------------------------
float Minimal::getParameter(VstInt32 index)
{
    return _gain;
}

//-----------------------------------------------------------------------------------------
void Minimal::getParameterName(VstInt32 index, char *label)
{
    strcpy_s(label, kVstMaxParamStrLen, "Gain");
}

//-----------------------------------------------------------------------------------------
void Minimal::getParameterDisplay(VstInt32 index, char *text)
{
    dB2string(_gain, text, kVstMaxParamStrLen);
}

//-----------------------------------------------------------------------------------------
void Minimal::getParameterLabel(VstInt32 index, char *label)
{
    strcpy_s(label, kVstMaxParamStrLen, "dB");
}

//------------------------------------------------------------------------
bool Minimal::getEffectName(char *name)
{
    strcpy_s(name, kVstMaxEffectNameLen, "Gain");
    return true;
}

//------------------------------------------------------------------------
bool Minimal::getProductString(char *text)
{
    strcpy_s(text, kVstMaxProductStrLen, "Gain");
    return true;
}

//------------------------------------------------------------------------
bool Minimal::getVendorString(char *text)
{
    strcpy_s(text, kVstMaxVendorStrLen, "Wouter");
    return true;
}

//-----------------------------------------------------------------------------------------
VstInt32 Minimal::getVendorVersion()
{
    return 1000;
}

//-----------------------------------------------------------------------------------------
void Minimal::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames)
{
    float *in1 = inputs[0];
    float *in2 = inputs[1];
    float *out1 = outputs[0];
    float *out2 = outputs[1];

    while (--sampleFrames >= 0)
    {
        (*out1++) = (*in1++) * _gain;
        (*out2++) = (*in2++) * _gain;
    }
}

//-----------------------------------------------------------------------------------------
void Minimal::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames)
{
    double *in1 = inputs[0];
    double *in2 = inputs[1];
    double *out1 = outputs[0];
    double *out2 = outputs[1];
    double dGain = _gain;

    while (--sampleFrames >= 0)
    {
        (*out1++) = (*in1++) * dGain;
        (*out2++) = (*in2++) * dGain;
    }
}
