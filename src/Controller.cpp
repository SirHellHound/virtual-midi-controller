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

#include "Controller.h"

namespace vmc {

struct Controller::Impl : public MidiKeyboardStateListener
{
    Impl() { }
    ~Impl() { }

    Settings settings;
    OptionalScopedPointer<AudioDeviceManager> devices;
    std::unique_ptr<MidiOutput> midiOut;
    MidiKeyboardState keyboardState;

    void handleNoteOn (MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override
    {
        midiOut->sendMessageNow (MidiMessage::noteOn (midiChannel, midiNoteNumber, velocity));
        if (auto* dout = devices->getDefaultMidiOutput())
            dout->sendMessageNow (MidiMessage::noteOn (midiChannel, midiNoteNumber, velocity));
    }

    void handleNoteOff (MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override
    {
        midiOut->sendMessageNow (MidiMessage::noteOff (midiChannel, midiNoteNumber, velocity));
        if (auto* dout = devices->getDefaultMidiOutput())
            dout->sendMessageNow (MidiMessage::noteOff (midiChannel, midiNoteNumber, velocity));
    }
};

Controller::Controller()
{
    impl.reset (new Impl());
    impl->devices.setOwned (new AudioDeviceManager ());
   #if JUCE_MAC
    impl->midiOut.reset (MidiOutput::createNewDevice ("VMC"));
    impl->midiOut->startBackgroundThread();
   #endif
    impl->keyboardState.addListener (impl.get());
}

Controller::~Controller()
{
    impl->keyboardState.removeListener (impl.get());
    if (impl->midiOut != nullptr)
        impl->midiOut->stopBackgroundThread();
    impl.reset();
}

void Controller::initializeAudioDevice()
{
    auto& devices = getDeviceManager();
    auto& settings = impl->settings;
    bool initDefault = true;
    
    if (auto* const props = settings.getUserSettings())
    {
        if (auto* xml = props->getXmlValue ("devices"))
        {
            initDefault = devices.initialise (32, 32, xml, false).isNotEmpty();
            deleteAndZero (xml);
        }
    }
    
    if (initDefault)
    {
        devices.initialiseWithDefaultDevices (32, 32);
    }

    devices.addAudioCallback (this);
    devices.addMidiInputCallback (String(), this);
}

void Controller::shutdown()
{
    auto& devices = getDeviceManager();
    devices.removeAudioCallback (this);
    devices.removeMidiInputCallback (String(), this);
    devices.closeAudioDevice();
}

void Controller::saveSettings()
{
    auto& settings = impl->settings;
    auto& devices  = getDeviceManager();

    if (auto* const props = settings.getUserSettings())
    {
        if (auto* devicesXml = devices.createStateXml())
        {
            props->setValue ("devices", devicesXml);
            deleteAndZero (devicesXml);
        }
    }
}

File Controller::getUserDataPath()
{
    auto path = File::getSpecialLocation (File::userMusicDirectory).getChildFile ("Controller");
    if (! path.exists())
        path.createDirectory();
    return path;
}

File Controller::getSamplesPath()
{
    auto path = getUserDataPath().getChildFile ("Samples");
    if (! path.exists())
        path.createDirectory();
    return path;
}

MidiKeyboardState& Controller::getMidiKeyboardState()         { return impl->keyboardState; }
Settings& Controller::getSettings()                           { return impl->settings; }
AudioDeviceManager& Controller::getDeviceManager()            { return *impl->devices; }

void Controller::addMidiMessage (const MidiMessage msg)
{
    impl->midiOut->sendMessageNow (msg);
    if (auto* const dout = impl->devices->getDefaultMidiOutput())
        dout->sendMessageNow (msg);
}

void Controller::audioDeviceIOCallback (const float**, int, float**,
                                        int, int) {}
void Controller::audioDeviceAboutToStart (AudioIODevice*) {}
void Controller::audioDeviceStopped() { }
void Controller::audioDeviceError (const String& errorMessage) { ignoreUnused (errorMessage); }

}
