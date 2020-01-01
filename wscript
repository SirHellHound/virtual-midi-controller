#!/usr/bin/env python

# This file is part of Virtual MIDI Controller
# Copyright (c) 2019  Kushview, LLC.  All rights reserved.

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

from subprocess import call, Popen, PIPE
import os, sys

sys.path.append (os.getcwd() + "/tools/waf")
import cross, vmc, juce

VERSION='0.1.0'

def options (opt):
    opt.load ("compiler_c compiler_cxx cross juce")

def silence_warnings (conf):
    '''TODO: resolve these'''
    conf.env.append_unique ('CFLAGS', ['-Wno-deprecated-register'])
    conf.env.append_unique ('CXXFLAGS', ['-Wno-deprecated-register'])
    conf.env.append_unique ('CFLAGS', ['-Wno-dynamic-class-memaccess'])
    conf.env.append_unique ('CXXFLAGS', ['-Wno-dynamic-class-memaccess'])
    conf.env.append_unique ('CFLAGS', ['-Wno-deprecated-declarations'])
    conf.env.append_unique ('CXXFLAGS', ['-Wno-deprecated-declarations'])

def configure_product_name (conf):
    return "Virtual MIDI Controller"

def configure (conf):
    conf.check_ccache()
    conf.load ("compiler_c compiler_cxx ar cross juce")
    conf.check_cxx_version()

    silence_warnings (conf)

    conf.check_common()
    if cross.is_mingw(conf): conf.check_mingw()
    elif juce.is_mac(): conf.check_mac()
    else: conf.check_linux()

    conf.env.DEBUG = conf.options.debug
    conf.env.VERSION_STRING = VERSION

    conf.define ('VERSION_STRING', conf.env.VERSION_STRING)
    conf.check_cfg (package='juce_audio_utils_debug-5' if conf.options.debug else 'juce_audio_utils-5', 
                    uselib_store='JUCE_AUDIO_UTILS', args='--cflags --libs', mandatory=True)
    conf.check_cfg (package='kv_gui_debug-0' if conf.options.debug else 'kv_gui-0', 
                    uselib_store='KV', args='--cflags --libs', mandatory=True)
    
    print
    juce.display_header ("Virtual MIDI Controller")
    print
    juce.display_msg (conf, "PREFIX", conf.env.PREFIX)
    juce.display_msg (conf, "DATADIR", conf.env.DATADIR)
    juce.display_msg (conf, "Debug", conf.options.debug)
    print
    juce.display_header ("Compiler")
    juce.display_msg (conf, "CFLAGS", conf.env.CFLAGS)
    juce.display_msg (conf, "CXXFLAGS", conf.env.CXXFLAGS)
    juce.display_msg (conf, "LINKFLAGS", conf.env.LINKFLAGS)

def build (bld):
    appEnv = bld.env.derive()
    app = bld.program (
        source      = bld.path.ant_glob ("src/**/*.cpp"),
        includes    = [ 'src', 'src/compat', 'libs/lua', 
                        'libs/lua/lua-5.3.5/src' ],
        target      = 'bin/virtual-midi-controller',
        name        = 'VMC',
        env         = appEnv,
        use         = [ 'KV', 'JUCE_AUDIO_UTILS' ]
    )

    if juce.is_mac():
        app.mac_app     = True,
        app.mac_plist   = 'tools/macdeploy/Info.plist'
        # app.mac_files   = [ 'project/Builds/MacOSX/Icon.icns' ]
        app.target      = 'Applications/MIDI Controller'

def macdeploy (ctx):
    call (["tools/macdeploy/appbundle.py",
           "-dmg", "vmc-osx-%s" % VERSION,
           "-volname", "VMC",
           "-fancy", "tools/macdeploy/fancy.plist",
           "build/Applications/MIDI Controller.app"])
