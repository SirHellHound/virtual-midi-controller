#pragma once
#include <juce/juce.h>
#include <kv/kv.h>

#if ! DONT_SET_USING_JUCE_NAMESPACE
 using namespace juce;
#endif

#if ! JUCE_DONT_DECLARE_PROJECTINFO
namespace ProjectInfo
{
    const char* const  projectName    = "Virtual MIDI Controller";
    const char* const  companyName    = "Kushview";
    const char* const  versionString  = "1.0.0";
    const int          versionNumber  = 0x10000;
}
#endif
