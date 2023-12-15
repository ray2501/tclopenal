# -*- tcl -*-
# Tcl package index file, version 1.1
#
if {[package vsatisfies [package provide Tcl] 9.0-]} {
    package ifneeded openal 0.7 \
	    [list load [file join $dir libtcl9openal0.7.so] [string totitle openal]]
} else {
    package ifneeded openal 0.7 \
	    [list load [file join $dir libopenal0.7.so] [string totitle openal]]
}
