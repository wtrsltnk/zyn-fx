#ifndef EFFECTPRESETS_H
#define EFFECTPRESETS_H

enum class EffectPresets
{
    Volume = 0,
    Panning = 1,
    LFOFrequency = 2,
    LFORandomness = 3,
    LFOFunction = 4,
    LFOStereo = 5,
};

enum class ReverbPresets
{
    ReverbTime = 2,
    ReverbInitialDelay = 3,
    ReverbInitialDelayFeedback = 4,
    ReverbUnused1 = 5,
    ReverbUnused2 = 6,
    ReverbLowPassFilter = 7,
    ReverbHighPassFilter = 8,
    ReverbDampening = 9,
    ReverbType = 10,
    ReverbRoomSize = 11,
    ReverbBandwidth = 12,
};

enum class EchoPresets
{
    EchoDelay = 2,
    EchoDelayBetweenLR = 3,
    EchoChannelRouting = 4,
    EchoFeedback = 5,
    EchoDampening = 6,
};

enum class ChorusPresets
{
    ChorusDepth = 6,
    ChorusDelay = 7,
    ChorusFeedback = 8,
    ChorusChannelRouting = 9,
    ChorusUnused1 = 10,
    ChorusSubtract = 11,
};

enum class PhaserPresets
{
    PhaserDepth = 6,
    PhaserFeedback = 7,
    PhaserStages = 8,
    PhaserChannelRouting = 9,
    PhaserSubtract = 10,
    PhaserPhase = 11,
    PhaserHyper = 12,
    PhaserDistortion = 13,
    PhaserAnalog = 14,
};

enum class AlienWahPresets
{
    AlienWahDepth = 6,
    AlienWahFeedback = 7,
    AlienWahDelay = 8,
    AlienWahChannelRouting = 9,
    AlienWahPhase = 10,
};

enum class DistorsionPresets
{
    DistorsionChannelRouting = 2,
    DistorsionDrive = 3,
    DistorsionLevel = 4,
    DistorsionType = 5,
    DistorsionNegate = 6,
    DistorsionLowPassFilter = 7,
    DistorsionHighPassFilter = 8,
    DistorsionStereo = 9,
    DistorsionPreFiltering = 10,
};

enum class EQPresets
{
    EQBandType = 10,
    EQBandFrequency = 11,
    EQBandGain = 12,
    EQBandQ = 13,
    EQBandStages = 14,
};

enum class DynFilterPresets
{
    DynFilterDepth = 6,
    DynFilterAmplitudeSense = 7,
    DynFilterAmplitudeSenseInvert = 8,
    DynFilterAmplitudeSmooth = 9,
};

#endif // EFFECTPRESETS_H
