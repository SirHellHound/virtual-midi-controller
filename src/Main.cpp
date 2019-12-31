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

#include "JuceHeader.h"
#include "MainComponent.h"
#include "LookAndFeel.h"
#include "Controller.h"

#ifndef VMC_WIDTH
 #define VMC_WIDTH   800
#endif

#ifndef VMC_HEIGHT
 #define VMC_HEIGHT  480
#endif

namespace vmc {

class Application  : public JUCEApplication
{
public:
    Application() { }

    const String getApplicationName() override       { return ProjectInfo::projectName; }
    const String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override       { return false; }

    void initialise (const String& commandLine) override
    {
        ignoreUnused (commandLine);
        setupGlobals();
        LookAndFeel::setDefaultLookAndFeel (&look);
        mainWindow.reset (new MainWindow (getApplicationName(), *controller));
    }

    void shutdown() override
    {
        shutdownGui();
        controller->saveSettings();
        controller->shutdown();
        controller.reset();
    }

    void systemRequestedQuit() override
    {
        quit();
    }

    void anotherInstanceStarted (const String& commandLine) override
    {
        ignoreUnused (commandLine);
    }

    class MainWindow : public DocumentWindow
    {
    public:
        MainWindow (String name, Controller& vc)
            : DocumentWindow (name, Desktop::getInstance().getDefaultLookAndFeel()
                                        .findColour (ResizableWindow::backgroundColourId), DocumentWindow::allButtons),
              controller (vc)
        {
            setUsingNativeTitleBar (true);
            setBackgroundColour (kv::LookAndFeel_KV1::widgetBackgroundColor.darker());
            setContentOwned (new MainComponent (vc), true);
            
            if (auto* props = controller.getSettings().getUserSettings())
            {
                const auto state = props->getValue ("windowPosition", String());
                if (state.isNotEmpty())
                    restoreWindowStateFromString (state);
            }

            setSize (VMC_WIDTH, VMC_HEIGHT);
            setContentComponentSize (VMC_WIDTH, VMC_HEIGHT);
            setResizable (false, false);

            setVisible (true);
        }

        ~MainWindow()
        {
            setConstrainer (nullptr);
        }

        void savePersistentData()
        {
            if (auto* props = controller.getSettings().getUserSettings())
            {
                props->setValue ("windowPosition", getWindowStateAsString());
            }

            if (auto* const comp = dynamic_cast<MainComponent*> (getContentComponent()))
            {
           
            }
        }

        void closeButtonPressed() override
        {
            JUCEApplication::getInstance()->systemRequestedQuit();
        }

        void maximiseButtonPressed() override
        {
            return;
        }

    private:
        Controller& controller;
        ComponentBoundsConstrainer constrain;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };

private:
    vmc::LookAndFeel look;
    std::unique_ptr<MainWindow> mainWindow;
    std::unique_ptr<Controller> controller;

    void setupGlobals()
    {
        controller.reset (new Controller());
        controller->initializeAudioDevice();
    }

    void shutdownGui()
    {
        if (mainWindow != nullptr)
        {
            mainWindow->savePersistentData();
            mainWindow = nullptr;
        }

        LookAndFeel::setDefaultLookAndFeel (nullptr);
    }
};

}

START_JUCE_APPLICATION (vmc::Application)
