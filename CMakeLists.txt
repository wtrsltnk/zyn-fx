cmake_minimum_required(VERSION 3.17)

project(zyn-fx-vst VERSION 1.0.0)

if(NOT DEFINED VST2_SDK_ROOT)
    message(FATAL_ERROR "The manatory variable VST2_SDK_ROOT is not defined. Please add this for access to the VST SDK version 2.4.")
endif()

find_package(OpenGL REQUIRED)

add_library(glad STATIC
    thirdparty/glad/src/glad.c
    thirdparty/glad/src/glad_wgl.c
)

target_include_directories(glad
    PUBLIC
        thirdparty/glad/include
)

target_link_libraries(glad
    PUBLIC
        ${OPENGL_LIBRARIES}
)


include(cmake/CPM.cmake)
include(cmake/Dependencies.cmake)

add_library(zyn STATIC
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
    src/Effects/Effect.cpp
    src/Effects/Effect.h
    src/Effects/EffectLFO.cpp
    src/Effects/EffectLFO.h
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
    src/vstcontrol.cpp
    src/vstcontrol.h
    # src/vstknob.cpp
    # src/vstknob.h
    ${VST2_SDK_ROOT}/public.sdk/source/vst2.x/audioeffectx.cpp
    ${VST2_SDK_ROOT}/public.sdk/source/vst2.x/audioeffect.cpp
    ${VST2_SDK_ROOT}/pluginterfaces/vst2.x/aeffectx.h
)

target_compile_features(zyn
    PRIVATE
        cxx_std_17
)

target_include_directories(zyn
    PUBLIC
        "include"
        "${VST2_SDK_ROOT}"
)

target_link_libraries(zyn
    PUBLIC
        glad
        imgui
)

##########################################################
############### All-In-One
##########################################################
add_library(zyn-all-in-one-vst MODULE
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
    src/zyn.all-in-one.cpp
    src/zyn.all-in-one.h
)

target_compile_definitions(zyn-all-in-one-vst
    PRIVATE
        -DZYN_ALIENWAH_VST=1
        -DZYN_CHORUS_VST=1
        -DZYN_DISTORSION_VST=1
        -DZYN_DYNAMICFILTER_VST=1
        -DZYN_EQ_VST=1
        -DZYN_ECHO_VST=1
        -DZYN_PHASER_VST=1
        -DZYN_REVERB_VST=1
)

target_link_libraries(zyn-all-in-one-vst
    zyn
)

##########################################################
############### Alien Wah effect
##########################################################
add_library(zyn-alienwah-vst MODULE
    resources/vstplug.def
    src/Effects/Alienwah.cpp
    src/Effects/Alienwah.h
    src/Effects/EffectMgr.cpp
    src/Effects/EffectMgr.h
    src/zyn.alienwah.cpp
    src/zyn.alienwah.h
    ${VST2_SDK_ROOT}/public.sdk/source/vst2.x/vstplugmain.cpp
)

target_compile_definitions(zyn-alienwah-vst
    PRIVATE
        -DZYN_ALIENWAH_VST=1
)

target_link_libraries(zyn-alienwah-vst
    zyn
    glad
    imgui
)


##########################################################
############### Chorus effect
##########################################################
add_library(zyn-chorus-vst MODULE
    resources/vstplug.def
    src/Effects/Chorus.cpp
    src/Effects/Chorus.h
    src/Effects/EffectMgr.cpp
    src/Effects/EffectMgr.h
    src/zyn.chorus.cpp
    src/zyn.chorus.h
    ${VST2_SDK_ROOT}/public.sdk/source/vst2.x/vstplugmain.cpp
)

target_compile_definitions(zyn-chorus-vst
    PRIVATE
        -DZYN_CHORUS_VST=1
)

target_link_libraries(zyn-chorus-vst
    zyn
)

##########################################################
############### Distorsion effect
##########################################################
add_library(zyn-distorsion-vst MODULE
    resources/vstplug.def
    src/Effects/Distorsion.cpp
    src/Effects/Distorsion.h
    src/Effects/EffectMgr.cpp
    src/Effects/EffectMgr.h
    src/zyn.distorsion.cpp
    src/zyn.distorsion.h
    ${VST2_SDK_ROOT}/public.sdk/source/vst2.x/vstplugmain.cpp
)

target_compile_definitions(zyn-distorsion-vst
    PRIVATE
        -DZYN_DISTORSION_VST=1
)

target_link_libraries(zyn-distorsion-vst
    zyn
)

##########################################################
############### DynamicFilter effect
##########################################################
add_library(zyn-dynamicfilter-vst MODULE
    resources/vstplug.def
    src/Effects/DynamicFilter.cpp
    src/Effects/DynamicFilter.h
    src/Effects/EffectMgr.cpp
    src/Effects/EffectMgr.h
    src/zyn.dynamicfilter.cpp
    src/zyn.dynamicfilter.h
    ${VST2_SDK_ROOT}/public.sdk/source/vst2.x/vstplugmain.cpp
)

target_compile_definitions(zyn-dynamicfilter-vst
    PRIVATE
        -DZYN_DYNAMICFILTER_VST=1
)

target_link_libraries(zyn-dynamicfilter-vst
    zyn
)

##########################################################
############### Echo effect
##########################################################
add_library(zyn-echo-vst MODULE
    resources/vstplug.def
    src/Effects/Echo.cpp
    src/Effects/Echo.h
    src/Effects/EffectMgr.cpp
    src/Effects/EffectMgr.h
    src/zyn.echo.cpp
    src/zyn.echo.h
    ${VST2_SDK_ROOT}/public.sdk/source/vst2.x/vstplugmain.cpp
)

target_compile_definitions(zyn-echo-vst
    PRIVATE
        -DZYN_ECHO_VST=1
)

target_link_libraries(zyn-echo-vst
    zyn
)

##########################################################
############### EQ effect
##########################################################
add_library(zyn-eq-vst MODULE
    resources/vstplug.def
    src/Effects/EffectMgr.cpp
    src/Effects/EffectMgr.h
    src/Effects/EQ.cpp
    src/Effects/EQ.h
    src/zyn.eq.cpp
    src/zyn.eq.h
    ${VST2_SDK_ROOT}/public.sdk/source/vst2.x/vstplugmain.cpp
)

target_compile_definitions(zyn-eq-vst
    PRIVATE
        -DZYN_EQ_VST=1
)

target_link_libraries(zyn-eq-vst
    zyn
)

##########################################################
############### Phaser effect
##########################################################
add_library(zyn-phaser-vst MODULE
    resources/vstplug.def
    src/Effects/EffectMgr.cpp
    src/Effects/EffectMgr.h
    src/Effects/Phaser.cpp
    src/Effects/Phaser.h
    src/zyn.phaser.cpp
    src/zyn.phaser.h
    ${VST2_SDK_ROOT}/public.sdk/source/vst2.x/vstplugmain.cpp
)

target_compile_definitions(zyn-phaser-vst
    PRIVATE
        -DZYN_PHASER_VST=1
)

target_link_libraries(zyn-phaser-vst
    zyn
)

##########################################################
############### Reverb effect
##########################################################
add_library(zyn-reverb-vst MODULE
    resources/vstplug.def
    src/Effects/EffectMgr.cpp
    src/Effects/EffectMgr.h
    src/Effects/Reverb.cpp
    src/Effects/Reverb.h
    src/zyn.reverb.cpp
    src/zyn.reverb.h
    ${VST2_SDK_ROOT}/public.sdk/source/vst2.x/vstplugmain.cpp
)

target_compile_definitions(zyn-reverb-vst
    PRIVATE
        -DZYN_REVERB_VST=1
)

target_link_libraries(zyn-reverb-vst
    zyn
)
