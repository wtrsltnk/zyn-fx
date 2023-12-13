#include "zyn.chorus.h"

#include <string.h>

#include "imgui_knob.h"
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

    // This is instance gets deleteed by AudioEffect desctructor
    _editor = new ZynEditor(this);

    setEditor(_editor);
    // default to 0 dB
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
            (*out1++) = (*procl++);
            (*out2++) = (*procr++);
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

static const char *chorusPresetNames[] = {
    "Chorus 1",
    "Chorus 2",
    "Chorus 3",
    "Celeste 1",
    "Celeste 2",
    "Flange 1",
    "Flange 2",
    "Flange 3",
    "Flange 4",
    "Flange 5",
};

ZynEditor::ZynEditor(
    ZynAudioEffectX *effect)
    : AEffEditor(effect),
      _main("ChorusControl"),
      _zynAudioEffectX(effect)
{}

bool ZynEditor::getRect(
    ERect **rect)
{
    *rect = &_rect;

    short w, h;
    if (_main.getSize(w, h))
    {
        _rect.left = 0;
        _rect.right = w;
        _rect.top = 0;
        _rect.bottom = h;

        return true;
    }

    return false;
}

void ZynEditor::Knob(
    EffectPresets par,
    const char *label,
    const char *tooltip)
{
    Knob(int(par), label, tooltip);
}

void ZynEditor::Knob(
    ChorusPresets par,
    const char *label,
    const char *tooltip)
{
    Knob(int(par), label, tooltip);
}

void ZynEditor::Knob(
    int par,
    const char *label,
    const char *tooltip)
{
    auto val = _zynAudioEffectX->EffectManager()->geteffectpar(par);

    if (ImGui::KnobUchar(label, &val, 0, 127, ImVec2(60, 40), tooltip ? tooltip : label))
    {
        _zynAudioEffectX->EffectManager()->seteffectpar(par, val);
    }
}

bool ZynEditor::open(
    void *ptr)
{
    auto hwnd = static_cast<HWND>(ptr);

    if (!_main.init(hwnd, 400, 290))
    {
        return false;
    }

    _main.renderUi = [&]() {
        ImGuiIO &io = ImGui::GetIO();
        (void)io;

        short w, h;
        _main.getSize(w, h);

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(w, h));

        auto flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize;
        ImGui::Begin("Chorus", nullptr, flags); // Create a window called "Hello, world!" and append into it.

        ImGui::SeparatorText("Chorus");

        static int item = -1;
        if (ImGui::Combo("Preset", &item, chorusPresetNames, IM_ARRAYSIZE(chorusPresetNames)))
        {
            _zynAudioEffectX->EffectManager()->changepreset(item);
        }

        ImGui::BeginGroup();
        {
            Knob(EffectPresets::Volume, "volume");
            ImGui::SameLine();
            Knob(EffectPresets::Panning, "pan");

            Knob(ChorusPresets::ChorusDelay, "delay");
            ImGui::SameLine();
            Knob(ChorusPresets::ChorusDepth, "depth");

            Knob(ChorusPresets::ChorusChannelRouting, "channel\nrouting", "channel routing");
            ImGui::SameLine();
            Knob(ChorusPresets::ChorusFeedback, "feedback");

            ImGui::EndGroup();
        }

        ImGui::SameLine();

        ImGui::BeginGroup();
        {
            if (ImGui::BeginChild("lfo", ImVec2(0, 90)))
            {
                ImGui::SeparatorText("lfo");

                Knob(EffectPresets::LFOFrequency, "frequency");
                ImGui::SameLine();
                Knob(EffectPresets::LFORandomness, "random");
                ImGui::SameLine();
                Knob(EffectPresets::LFOStereo, "stereo");
            }
            ImGui::EndChild();

            bool b = _zynAudioEffectX->EffectManager()->geteffectpar(int(ChorusPresets::ChorusSubtract)) != 0;
            if (ImGui::Checkbox("subtract", &b))
            {
                _zynAudioEffectX->EffectManager()->seteffectpar(int(ChorusPresets::ChorusSubtract), b ? 1 : 0);
            }

            ImGui::EndGroup();
        }

        ImGui::End();
    };

    return true;
}

void ZynEditor::close()
{
    _main.close();
}

bool ZynEditor::isOpen()
{
    return _main.isOpen();
}

void ZynEditor::idle() {}
