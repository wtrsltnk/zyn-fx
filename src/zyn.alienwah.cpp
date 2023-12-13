#include "zyn.alienwah.h"

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
    setUniqueID('ZWAH');   // identify
    canProcessReplacing(); // supports replacing output

    // This is instance gets deleteed by AudioEffect desctructor
    _editor = new ZynEditor(this);

    setEditor(_editor);

    _gain = 1.f;                                           // default to 0 dB
    strcpy_s(_programName, kVstMaxProgNameLen, "Default"); // default program name

    _effectMgr = std::make_unique<EffectMgr>(&_mutex);

    _effectMgr->changeeffect(EffectTypes::AlienwahType);
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
    strcpy_s(name, kVstMaxEffectNameLen, "Zyn.Alienwah");
    return true;
}

//------------------------------------------------------------------------
bool ZynAudioEffectX::getProductString(char *text)
{
    strcpy_s(text, kVstMaxProductStrLen, "Zyn.Alienwah");
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

std::vector<std::string> EffectNames = {
    "No effect",
    "Reverb",
    "Echo",
    "Chorus",
    "Phaser",
    "AlienWah",
    "Distortion",
    "Equalizer",
    "DynFilter",
};

static std::vector<std::string> reverbPresetNames = {
    "Cathedral 1",
    "Cathedral 2",
    "Cathedral 3",
    "Hall 1",
    "Hall 2",
    "Room 1",
    "Room 2",
    "Basement",
    "Tunnel",
    "Echoed 1",
    "Echoed 2",
    "Very Long 1",
    "Very Long 2",
};

static std::vector<std::string> echoPresetNames = {
    "Echo 1",
    "Echo 2",
    "Echo 3",
    "Simple Echo",
    "Canyon",
    "Panning Echo 1",
    "Panning Echo 2",
    "Panning Echo 3",
    "Feedback Echo",
};

static std::vector<std::string> phaserPresetNames = {
    "Phaser 1",
    "Phaser 2",
    "Phaser 3",
    "Phaser 4",
    "Phaser 5",
    "Phaser 6",
    "APhaser 1",
    "APhaser 2",
    "APhaser 3",
    "APhaser 4",
    "APhaser 5",
    "APhaser 6",
};

static const char *alienWahPresetNames[] = {
    "Alien Wah 1",
    "Alien Wah 2",
    "Alien Wah 3",
    "Alien Wah 4",
};

static std::vector<std::string> distortionPresetNames = {
    "Overdrive 1",
    "Overdrive 2",
    "A. Exciter 1",
    "A. Exciter 2",
    "Guitar Amp",
    "Quantisize",
};

static std::vector<std::string> dynFilterPresetNames = {
    "WahWah",
    "AutoWah",
    "Sweep",
    "VocalMorph 1",
    "VocalMorph 2",
};

static std::vector<std::string> reverbTypeNames = {
    "Random",
    "Freeverb",
    "Bandwidth",
};

static int const lfoTypeCount = 2;
static std::vector<std::string> lfoTypes = {
    "SINE",
    "TRI",
};

static std::vector<std::string> distortionTypes = {
    "Atan",
    "Asym1",
    "Pow",
    "Sine",
    "Qnts",
    "Zigzg",
    "Lmt",
    "LmtU",
    "LmtL",
    "ILmt",
    "Clip",
    "Asym2",
    "Pow2",
    "Sign",
};

static std::vector<std::string> eqBandTypes = {
    "Off",
    "Lp1",
    "Hp1",
    "Lp2",
    "Hp2",
    "Bp2",
    "N2",
    "Pk",
    "LSh",
    "HSh",
};

ZynEditor::ZynEditor(
    ZynAudioEffectX *effect)
    : AEffEditor(effect),
      _main("AlienWahControl"),
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
    AlienWahPresets par,
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
        ImGui::Begin("AlienWah", nullptr, flags); // Create a window called "Hello, world!" and append into it.

        ImGui::SeparatorText("AlienWah");

        static int item = -1;
        if (ImGui::Combo("Preset", &item, alienWahPresetNames, IM_ARRAYSIZE(alienWahPresetNames)))
        {
            _zynAudioEffectX->EffectManager()->changepreset(item);
        }

        ImGui::BeginGroup();
        {
            Knob(EffectPresets::Volume, "volume");
            ImGui::SameLine();
            Knob(EffectPresets::Panning, "pan");

            Knob(AlienWahPresets::AlienWahPhase, "phase");
            ImGui::SameLine();
            Knob(AlienWahPresets::AlienWahDepth, "depth");

            Knob(AlienWahPresets::AlienWahChannelRouting, "channel\nrouting", "channel routing");
            ImGui::SameLine();
            Knob(AlienWahPresets::AlienWahFeedback, "feedback");

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

            ImGui::SeparatorText("delay");

            unsigned char u8_min = 1, u8_max = 127;

            auto val = _zynAudioEffectX->EffectManager()->geteffectpar(int(AlienWahPresets::AlienWahDelay));
            if (ImGui::SliderScalar("delay", ImGuiDataType_U8, &val, &u8_min, &u8_max, "%u"))
            {
                _zynAudioEffectX->EffectManager()->seteffectpar(int(AlienWahPresets::AlienWahDelay), val);
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
