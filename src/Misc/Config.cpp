/*
  ZynAddSubFX - a software synthesizer

  Config.cpp - Configuration file functions
  Copyright (C) 2003-2005 Nasca Octavian Paul
  Author: Nasca Octavian Paul

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License
  as published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License (version 2 or later) for more details.

  You should have received a copy of the GNU General Public License (version 2)
  along with this program; if not, write to the Free Software Foundation,
  Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

*/
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Config.h"

using namespace std;

Config::Config()
{}
void Config::init()
{
    maxstringsize = MAX_STRING_SIZE; // for ui
    // defaults
    cfg.SampleRate = 44100;
    cfg.SoundBufferSize = 256;
    cfg.OscilSize = 1024;
    cfg.SwapStereo = 0;

    cfg.LinuxOSSWaveOutDev = new char[MAX_STRING_SIZE];
    snprintf(cfg.LinuxOSSWaveOutDev, MAX_STRING_SIZE, "/dev/dsp");
    cfg.LinuxOSSSeqInDev = new char[MAX_STRING_SIZE];
    snprintf(cfg.LinuxOSSSeqInDev, MAX_STRING_SIZE, "/dev/sequencer");

    cfg.DumpFile = "zynaddsubfx_dump.txt";

    cfg.WindowsWaveOutId = 0;
    cfg.WindowsMidiInId = 0;

    cfg.BankUIAutoClose = 0;
    cfg.DumpNotesToFile = 0;
    cfg.DumpAppend = 1;

    cfg.GzipCompression = 3;

    cfg.Interpolation = 0;
    cfg.CheckPADsynth = 1;
    cfg.IgnoreProgramChange = 0;

    cfg.UserInterfaceMode = 0;
    cfg.VirKeybLayout = 1;
    winwavemax = 1;
    winmidimax = 1;
    // try to find out how many input midi devices are there
    winmididevices = new winmidionedevice[winmidimax];
    for (int i = 0; i < winmidimax; ++i)
    {
        winmididevices[i].name = new char[MAX_STRING_SIZE];
        for (int j = 0; j < MAX_STRING_SIZE; ++j)
            winmididevices[i].name[j] = '\0';
    }

    // get the midi input devices name
    cfg.currentBankDir = "./testbnk";

    char filename[MAX_STRING_SIZE];
    getConfigFileName(filename, MAX_STRING_SIZE);
    readConfig(filename);

    if (cfg.bankRootDirList[0].empty())
    {
        // banks
        cfg.bankRootDirList[0] = "~/banks";
        cfg.bankRootDirList[1] = "./";
        cfg.bankRootDirList[2] = "/usr/share/zynaddsubfx/banks";
        cfg.bankRootDirList[3] = "/usr/local/share/zynaddsubfx/banks";
#ifdef __APPLE__
        cfg.bankRootDirList[4] = "../Resources/banks";
#else
        cfg.bankRootDirList[4] = "../banks";
#endif
        cfg.bankRootDirList[5] = "banks";
    }

    if (cfg.presetsDirList[0].empty())
    {
        // presets
        cfg.presetsDirList[0] = "./";
#ifdef __APPLE__
        cfg.presetsDirList[1] = "../Resources/presets";
#else
        cfg.presetsDirList[1] = "../presets";
#endif
        cfg.presetsDirList[2] = "presets";
        cfg.presetsDirList[3] = "/usr/share/zynaddsubfx/presets";
        cfg.presetsDirList[4] = "/usr/local/share/zynaddsubfx/presets";
    }
    cfg.LinuxALSAaudioDev = "default";
    cfg.nameTag = "";
}

Config::~Config()
{
    delete[] cfg.LinuxOSSWaveOutDev;
    delete[] cfg.LinuxOSSSeqInDev;

    for (int i = 0; i < winmidimax; ++i)
        delete[] winmididevices[i].name;
    delete[] winmididevices;
}

void Config::save()
{
    char filename[MAX_STRING_SIZE];
    getConfigFileName(filename, MAX_STRING_SIZE);
    saveConfig(filename);
}

void Config::clearbankrootdirlist()
{
    for (int i = 0; i < MAX_BANK_ROOT_DIRS; ++i)
        cfg.bankRootDirList[i].clear();
}

void Config::clearpresetsdirlist()
{
    for (int i = 0; i < MAX_BANK_ROOT_DIRS; ++i)
        cfg.presetsDirList[i].clear();
}

void Config::readConfig(const char *filename)
{

    cfg.OscilSize = (int)powf(2, ceil(logf(cfg.OscilSize - 1.0f) / logf(2.0f)));
}

void Config::saveConfig(const char *filename)
{
}

void Config::getConfigFileName(char *name, int namesize)
{
    name[0] = 0;
    snprintf(name, namesize, "%s%s", getenv("HOME"), "/.zynaddsubfxXML.cfg");
}
