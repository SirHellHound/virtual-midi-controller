/*
    This file is part of Virtual MIDI Controller
    Copyright (c) 2019  Kushview, LLC.  All rights reserved.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#pragma once

#include "JuceHeader.h"
#include "Settings.h"

namespace vmc {

class Controller final : public AudioIODeviceCallback,
                         public MidiInputCallback
{
public:
    Controller();
    ~Controller();

    //=========================================================================
    Settings& getSettings();
    void saveSettings();

    //=========================================================================
    void addMidiMessage (const MidiMessage msg);
    MidiKeyboardState& getMidiKeyboardState();

    //=========================================================================
    static File getUserDataPath();
    static File getSamplesPath();
    
    //=========================================================================
    AudioDeviceManager& getDeviceManager();

    //=========================================================================
    void audioDeviceIOCallback (const float** inputChannelData,
                                int numInputChannels, float** outputChannelData,
                                int numOutputChannels, int numSamples) override;
    void audioDeviceAboutToStart (AudioIODevice* device) override;
    void audioDeviceStopped() override;
    void audioDeviceError (const String& errorMessage) override;

    //=========================================================================
    virtual void handleIncomingMidiMessage (MidiInput*, const MidiMessage&) override {}
    virtual void handlePartialSysexMessage (MidiInput*, const uint8*,
                                            int, double) override {}

private:
    struct Impl; std::unique_ptr<Impl> impl;

    //=========================================================================
    friend class Application;
    void initializeAudioDevice();
    void shutdown();
};

}
