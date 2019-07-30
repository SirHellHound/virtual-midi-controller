#pragma once
#include <juce/juce.h>
#include <kv_core/kv_core.h>
#include <kv_gui/kv_gui.h>
#include <kv_models/kv_models.h>


#if ! DONT_SET_USING_JUCE_NAMESPACE
 // If your code uses a lot of JUCE classes, then this will obviously save you
 // a lot of typing, but can be disabled by setting DONT_SET_USING_JUCE_NAMESPACE.
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
