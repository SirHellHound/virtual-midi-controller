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
    void initializeAudioDevice();
    void initializePlugins();
    void shutdown();

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
    AudioPluginFormatManager& getPluginManager();
    AudioFormatManager& getAudioFormats();

    //=========================================================================
    void audioDeviceIOCallback (const float** inputChannelData,
                                int numInputChannels, float** outputChannelData,
                                int numOutputChannels, int numSamples) override;
    void audioDeviceAboutToStart (AudioIODevice* device) override;
    void audioDeviceStopped() override;
    void audioDeviceError (const String& errorMessage) override;

    virtual void handleIncomingMidiMessage (MidiInput* source,
                                            const MidiMessage& message) override {}

    virtual void handlePartialSysexMessage (MidiInput* source,
                                            const uint8* messageData,
                                            int numBytesSoFar,
                                            double timestamp) override {}

private:
    struct Impl; std::unique_ptr<Impl> impl;
};

}
