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

#include "MainComponent.h"
#include "VirtualKeyboard.h"
#include "Controller.h"

namespace vmc {

class MainComponent::Content : public Component
{
public:
    Content (MainComponent& o)
        : owner (o),
          keyboard (o.controller.getMidiKeyboardState())
    {
        setOpaque (true);

        addAndMakeVisible (slider1);
        slider1.setRange (0.0, 127.0, 1.0);
        slider1.setSliderStyle (Slider::LinearVertical);
        slider1.onValueChange = [this]()
        {
            int value = roundToInt (slider1.getValue());
            owner.controller.addMidiMessage (MidiMessage::controllerEvent (
                midiChannel, 60, value));
        };

        addAndMakeVisible (slider2);
        slider2.setRange (0.0, 127.0, 1.0);
        slider2.setSliderStyle (Slider::LinearVertical);
        slider2.onValueChange = [this]()
        {
            int value = roundToInt (slider2.getValue());
            owner.controller.addMidiMessage (MidiMessage::controllerEvent (
                midiChannel, 61, value));
        };

        addAndMakeVisible (slider3);
        slider3.setRange (0.0, 127.0, 1.0);
        slider3.setSliderStyle (Slider::LinearVertical);
        slider3.onValueChange = [this]()
        {
            int value = roundToInt (slider3.getValue());
            owner.controller.addMidiMessage (MidiMessage::controllerEvent (
                midiChannel, 62, value));
        };

        addAndMakeVisible (keyboard);
        
        addAndMakeVisible (program);
        program.setTooltip ("MIDI Program");
        program.setRange (1.0, 128.0, 1.0);
        program.setSliderStyle (Slider::IncDecButtons);
        program.setValue (1.0, dontSendNotification);
        program.onValueChange = [this]()
        {
            int value = jlimit (1, 128, roundToInt (program.getValue())) - 1;
            owner.controller.getSettings().set (Settings::lastMidiProgram, value);
            owner.controller.addMidiMessage (MidiMessage::programChange (midiChannel, value));
        };

        addAndMakeVisible (channel);
        channel.setTooltip ("MIDI Channel");
        channel.setRange (1.0, 16.0, 1.0);
        channel.setSliderStyle (Slider::IncDecButtons);
        channel.setValue (1.0, dontSendNotification);
        channel.onValueChange = [this]()
        {
            midiChannel = roundToInt (channel.getValue());
            owner.controller.getSettings().set (Settings::lastMidiChannel, midiChannel);
            keyboard.setMidiChannel (midiChannel);
        };

        addAndMakeVisible (output);
        output.onChange = [this]()
        {
            auto& devices = owner.controller.getDeviceManager();

            if (output.getSelectedId() == 1)
                devices.setDefaultMidiOutput (String());
            else
                devices.setDefaultMidiOutput (output.getText());
        };

        setSize (440, 340);

        updateMidiOutputs();
        updateWithSettings();
    }

    ~Content()
    {
        slider1.onValueChange = nullptr;
        slider2.onValueChange = nullptr;
        slider3.onValueChange = nullptr;
        program.onValueChange = nullptr;
        channel.onValueChange = nullptr;
        output.onChange = nullptr;
    }

    void updateWithSettings()
    {
        auto& settings = owner.controller.getSettings();

        midiChannel = settings.getInt (Settings::lastMidiChannel, 1);
        channel.setValue ((double) midiChannel, dontSendNotification);
        program.setValue (1.0 + (double) settings.getInt (Settings::lastMidiProgram, 0), dontSendNotification);

        const String name = owner.controller.getDeviceManager().getDefaultMidiOutputName();
        if (name.isEmpty())
        {
            output.setSelectedId (1, dontSendNotification);
        }
        else
        {
            for (int i = output.getNumItems(); --i >= 0;)
            {
                if (output.getItemText (i) == name)
                {
                    output.setSelectedItemIndex (i, dontSendNotification);
                    break;
                }
            }
        }
    }

    void paint (Graphics& g) override
    {
        g.fillAll (kv::LookAndFeel_KV1::widgetBackgroundColor.darker());
    }

    void resized() override
    {
        auto r = getLocalBounds();
        auto r2 = r.removeFromTop (18);
        channel.setBounds (r2.removeFromLeft (64));
        program.setBounds (r2.removeFromLeft (64));
        output.setBounds (r2.removeFromRight (120));

        slider1.setBounds (r.removeFromLeft (18));
        slider2.setBounds (r.removeFromLeft (18));
        // slider3.setBounds (r.removeFromRight (18));
        keyboard.setBounds (r);
    }

    void updateMidiOutputs()
    {
        output.clear (dontSendNotification);
       
        output.addItem ("None", 1);
        output.addSeparator();

        int index = 0;
        for (const auto& name : MidiOutput::getDevices())
            output.addItem (name, 1000 + index);
    }

private:
    MainComponent& owner;
    VirtualKeyboard keyboard;
    Slider slider1, slider2, slider3;
    Slider program, channel;
    ComboBox output;

    int midiChannel = 1;
};

MainComponent::MainComponent (Controller& vc)
    : controller (vc)
{
    setOpaque (true);
    content.reset (new Content (*this));
    addAndMakeVisible (content.get());
    setSize (600, 400);
}

MainComponent::~MainComponent()
{
    content.reset();
}

void MainComponent::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (DocumentWindow::backgroundColourId));
}

void MainComponent::resized()
{
    content->setBounds (getLocalBounds());
}

}
