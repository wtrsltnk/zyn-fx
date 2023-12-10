cmake_minimum_required(VERSION 3.17)

project(zyn-fx-vst VERSION 1.0.0)

if(NOT DEFINED VST2_SDK_ROOT)
    message(FATAL_ERROR "The manatory variable VST2_SDK_ROOT is not defined. Please add this for access to the VST SDK version 2.4.")
endif()

message (STATUS ${VST2_SDK_ROOT})

configure_file(config.h.in config.h)

add_library(zyn-fx-vst MODULE
    resources/vstplug.def
    src/DSP/AnalogFilter.cpp
    src/DSP/AnalogFilter.h
    src/DSP/Filter.cpp
    src/DSP/Filter.h
    src/DSP/FormantFilter.cpp
    src/DSP/FormantFilter.h
    src/DSP/SVFilter.cpp
    src/DSP/SVFilter.h
    src/DSP/Unison.cpp
    src/DSP/Unison.h
    src/Effects/Alienwah.cpp
    src/Effects/Alienwah.h
    src/Effects/Chorus.cpp
    src/Effects/Chorus.h
    src/Effects/Distorsion.cpp
    src/Effects/Distorsion.h
    src/Effects/DynamicFilter.cpp
    src/Effects/DynamicFilter.h
    src/Effects/EQ.cpp
    src/Effects/EQ.h
    src/Effects/Echo.cpp
    src/Effects/Echo.h
    src/Effects/Effect.cpp
    src/Effects/Effect.h
    src/Effects/EffectLFO.cpp
    src/Effects/EffectLFO.h
    src/Effects/EffectMgr.cpp
    src/Effects/EffectMgr.h
    src/Effects/Phaser.cpp
    src/Effects/Phaser.h
    src/Effects/Reverb.cpp
    src/Effects/Reverb.h
    src/Misc/Config.cpp
    src/Misc/Config.h
    src/Misc/Stereo.h
    src/Misc/Util.cpp
    src/Misc/Util.h
    src/Misc/WaveShapeSmps.cpp
    src/Misc/WaveShapeSmps.h
    src/Params/FilterParams.cpp
    src/Params/FilterParams.h
    src/globals.h
    src/instrumentmain.cpp
    src/minimal.h
    ${VST2_SDK_ROOT}/public.sdk/source/vst2.x/audioeffectx.cpp
    ${VST2_SDK_ROOT}/public.sdk/source/vst2.x/audioeffect.cpp
    ${VST2_SDK_ROOT}/public.sdk/source/vst2.x/vstplugmain.cpp
    ${VST2_SDK_ROOT}/pluginterfaces/vst2.x/aeffectx.h
)

target_compile_features(zyn-fx-vst
    PRIVATE
        cxx_nullptr
)

target_include_directories(zyn-fx-vst
    PRIVATE
        "include"
    PUBLIC
        "${PROJECT_BINARY_DIR}"
        "${VST2_SDK_ROOT}"
)
