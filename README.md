# Virtual MIDI Controller
![Screenshot](data/screenshot.png)

A software MIDI controller which can send MIDI to any input device.  Also exposes itself as a MIDI input to other applications (OSX only)

## Building with Projucer
This is a JUCE project and should compile no problems by normal means.  However, the prefered method is to use Waf or VSCode.  See below.

## Building with Waf
Note: Open the `jucer/VMC.jucer` with Projucer and save it before doing this.  Generated files currently are not saved in Git.
```
./waf configure build
```
Run `./waf --help` for all options.

## Building & Debugging with VSCode
There are launch and build tasks ready to go in the `.vscode` folder.  You _should_ see them in the tasks dialog.
