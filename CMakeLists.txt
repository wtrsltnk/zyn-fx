cmake_minimum_required(VERSION 3.17)

project(zyn-chorus-vst VERSION 1.0.0)

if(NOT DEFINED VST2_SDK_ROOT)
    message(FATAL_ERROR "The manatory variable VST2_SDK_ROOT is not defined. Please add this for access to the VST SDK version 2.4.")
endif()

message (STATUS ${VST2_SDK_ROOT})

configure_file(config.h.in config.h)

add_library(zyn-chorus-vst MODULE
    src/minimal.h
    src/instrumentmain.cpp
    resources/vstplug.def
    ${VST2_SDK_ROOT}/public.sdk/source/vst2.x/audioeffectx.cpp
    ${VST2_SDK_ROOT}/public.sdk/source/vst2.x/audioeffect.cpp
    ${VST2_SDK_ROOT}/public.sdk/source/vst2.x/vstplugmain.cpp
    ${VST2_SDK_ROOT}/pluginterfaces/vst2.x/aeffectx.h
)

target_compile_features(zyn-chorus-vst
    PRIVATE
        cxx_nullptr
)

target_include_directories(zyn-chorus-vst
    PRIVATE
        "include"
    PUBLIC
        "${PROJECT_BINARY_DIR}"
        "${VST2_SDK_ROOT}"
)
