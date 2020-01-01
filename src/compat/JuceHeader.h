// Compatibility header
#pragma once

#include <juce/juce.h>
#if HAVE_KV
#include <kv/gui.h>
#endif

using namespace juce;

namespace ProjectInfo
{
    const char* const  projectName    = "Virtual MIDI Controller";
    const char* const  companyName    = "Kushview";
    const char* const  versionString  = "0.1.0";
    const int          versionNumber  = 0x100;
}
