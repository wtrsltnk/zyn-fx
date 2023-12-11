#pragma once

#include "Effects/EffectMgr.h"
#include <memory>
#include <mutex>
#include <public.sdk/source/vst2.x/audioeffectx.h>

class ZynAudioEffectX : public AudioEffectX
{
public:
    ZynAudioEffectX(
        audioMasterCallback audioMaster);
    
    ~ZynAudioEffectX();

    virtual void processReplacing(
        float **inputs,
        float **outputs,
        VstInt32 sampleFrames);

    virtual void setProgramName(
        char *name);

    virtual void getProgramName(
        char *name);

    virtual void setParameter(
        VstInt32 index,
        float value);

    virtual float getParameter(
        VstInt32 index);

    virtual void getParameterLabel(
        VstInt32 index,
        char *label);

    virtual void getParameterDisplay(
        VstInt32 index,
        char *text);

    virtual void getParameterName(
        VstInt32 index,
        char *text);

    virtual bool getEffectName(
        char *name);

    virtual bool getVendorString(
        char *text);

    virtual bool getProductString(
        char *text);

    virtual VstInt32 getVendorVersion();

private:
    float _gain = 1.0f;
    char _programName[kVstMaxProgNameLen + 1];
    std::unique_ptr<EffectMgr> _effectMgr;
    std::mutex _mutex;
};
