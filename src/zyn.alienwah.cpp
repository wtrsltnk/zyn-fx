#include "zyn.alienwah.h"

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

ZynEditor::ZynEditor(
    AudioEffect *effect)
    : AEffEditor(effect)
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

bool ZynEditor::open(
    void *ptr)
{
    auto hwnd = static_cast<HWND>(ptr);

    if (!_main.init(hwnd, 400, 200))
    {
        return false;
    }

    static float f = 0.0f;
    static int counter = 0;
    static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    _main.renderUi = [&]() {
        ImGuiIO &io = ImGui::GetIO();
        (void)io;

        ImGui::SetNextWindowPos(ImVec2(0, 0));

        ImGui::Begin("Hello, plugn!"); // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text."); // Display some text (you can use a format strings too)

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);             // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float *)&clear_color); // Edit 3 floats representing a color

        if (ImGui::Button("Button")) // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
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
    return false;
}

void ZynEditor::idle() {}
