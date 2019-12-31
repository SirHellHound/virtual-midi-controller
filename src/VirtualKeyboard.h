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

namespace vmc {

class VirtualKeyboard : public Component
{
public:
    VirtualKeyboard (MidiKeyboardState& state)
    {
        keyboard.reset (new MidiKeyboardComponent (state, MidiKeyboardComponent::horizontalKeyboard));
        addAndMakeVisible (keyboard.get());
    }

    ~VirtualKeyboard()
    {
        keyboard = nullptr;
    }

    /** Set the MIDI channel to use */
    void setMidiChannel (int channel) { keyboard->setMidiChannel (channel); }

    /** Get the current MIDI channel */
    int getMidiChannel() const noexcept { return keyboard->getMidiChannel(); }

    void resized() override
    {
        keyboard->setBounds (getLocalBounds());
    }

    MidiKeyboardComponent& getMidiKeyboardComponent() { return *keyboard; }

private:
    std::unique_ptr<MidiKeyboardComponent> keyboard;
};

}
