tclopenal
=====

[OpenAL](https://www.openal.org/) is a library for creation of a virtual
3D world of sound. It is a cross-platform audio application programming
interface (API). The application programmer can specify the location,
the speed and the direction of the sources of sounds and of the listener.

[OpenAL Soft](http://kcat.strangesoft.net/openal.html) is an LGPL-licensed,
cross-platform, software implementation of the OpenAL 3D audio API.



It is a Tcl bindings for OpenAL (open audio library).
Now only support playback function.
Capture function added to current codebase but not actually verify.


License
=====

LGPL 2.1, or (at your option) any later version.


Commands
=====

openal::list_devices  
openal::list_captures  
openal::device HANDLE ?-device name?  
HANDLE setListener property setting  
HANDLE getListener property  
HANDLE createSource  
HANDLE isValidSource  
HANDLE setSource property setting  
HANDLE getSource property  
HANDLE queueBuffers ?number?  
HANDLE unqueueBuffers ?number?  
HANDLE playSource  
HANDLE pauseSource  
HANDLE rewindSource  
HANDLE stopSource  
HANDLE destroySource  
HANDLE createBuffer ?number?  
HANDLE isValidBuffer ?index?  
HANDLE bufferData format data samplerate ?index?  
HANDLE getBuffer property ?index?  
HANDLE destroyBuffer  
HANDLE close  
openal::capture CAPTURE ?-device name? ?-rate samplerate? ?-format format? ?-buffersize size?  
CAPTURE start   
CAPTURE captureSamples  
CAPTURE stop  
CAPTURE close

Open AL provides an extension querying mechanism which allows you to know
whether the runtime Open AL implementation supports a specific extension.
If the enumeration extension is supported, we can procede with listing
the audio devices by using `openal::list_devices` command.

And `openal::list_captures` list the capture devices.

## openal::device

`openal::device` is to open a handle to a device, then create and initialize
a context.

`setListener` setup the listener object property, AL_GAIN (float),
AL_POSITION (list of float, float, float),
AL_VELOCITY (list of float, float, float),
AL_ORIENTATION (list of float, float, float, float, float, float)

`getListener` retrieve the listener property, AL_GAIN (float),
AL_POSITION (list of float, float, float),
AL_VELOCITY (list of float, float, float),
AL_ORIENTATION (list of float, float, float, float, float, float)

To playback audio, we must create an audio source object, so we can use the
`createSource` method to create a source object.

`setSource` setup the source property,
AL_BUFFER, AL_LOOPING (boolean), AL_SOURCE_RELATIVE (boolean),
AL_SOURCE_STATE (string: AL_INITIAL, AL_PLAYING, AL_PAUSED and AL_STOPPED),
AL_PITCH (float), AL_GAIN (float), AL_MIN_GAIN (float), AL_MAX_GAIN (float),
AL_MAX_DISTANCE (float), AL_ROLLOFF_FACTOR (float),
AL_CONE_OUTER_GAIN (float), AL_CONE_INNER_ANGLE (float),
AL_CONE_OUTER_ANGLE (float), AL_REFERENCE_DISTANCE (float),
AL_SEC_OFFSET (float), AL_SAMPLE_OFFSET (float),
AL_POSITION (list of float, float, float),
AL_VELOCITY (list of float, float, float),
AL_ORIENTATION (list of float, float, float, float, float, float)

`getSource` retrieve the source property, AL_SOURCE_RELATIVE (boolean),
AL_PITCH (float), AL_GAIN (float), AL_MIN_GAIN (float), AL_MAX_GAIN (float),
AL_MAX_DISTANCE (float), AL_ROLLOFF_FACTOR (float),
AL_CONE_OUTER_GAIN (float), AL_CONE_INNER_ANGLE (float),
AL_CONE_OUTER_ANGLE (float), AL_REFERENCE_DISTANCE (float),
AL_BUFFERS_PROCESSED (int), AL_BUFFERS_QUEUED (int),
AL_SEC_OFFSET (float), AL_SAMPLE_OFFSET (float),
AL_POSITION (list of float, float, float),
AL_VELOCITY (list of float, float, float),
AL_ORIENTATION (list of float, float, float, float, float, float)

`destroySource` is to destory source.

The buffer object is the object actually holding the raw audio stream.
Use the `createBuffer` method to create a buffer object.

User can specify the number of buffer to create.
If user does not specify, the default number is 1.

`bufferData` format has values:
AL_FORMAT_MONO8, AL_FORMAT_MONO16, AL_FORMAT_STEREO8,
AL_FORMAT_STEREO16

`getBuffer` retrieve the buffer property, AL_FREQUENCY, AL_BITS, AL_CHANNELS
and AL_SIZE.

`destroyBuffer` is to destory buffer.

## openal::capture

`openal::capture` is to open a handle to a capture device.
The `-buffersize` argument specifies the number of sample frames.
`-format` has values: AL_FORMAT_MONO8, AL_FORMAT_MONO16, AL_FORMAT_STEREO8,
AL_FORMAT_STEREO16.


UNIX BUILD
=====

I only test tclopenal under openSUSE LEAP 42.2 and Ubuntu 14.04.

Users need install libopenal development files.
Below is an example for openSUSE:

	sudo zypper in openal-soft-devel

Below is an example for Ubuntu:

	sudo apt-get install libopenal-dev

Building under most UNIX systems is easy, just run the configure script
and then run make. For more information about the build process, see the
tcl/unix/README file in the Tcl src dist. The following minimal example
will install the extension in the /opt/tcl directory.

	$ cd tclopenal
	$ ./configure --prefix=/opt/tcl
	$ make
	$ make install

If you need setup directory containing tcl configuration (tclConfig.sh),
below is an example:

	$ cd tclopenal
	$ ./configure --with-tcl=/opt/activetcl/lib
	$ make
	$ make install


WINDOWS BUILD
=====

## MSYS2/MinGW-W64

Using pacman to install OpenAL Soft.

	$ pacman -S mingw-w64-x86_64-openal

Put libopenal-1.dll to Windows folder or other available folder.

Next step is to build tclopenal.

	$ ./configure --with-tcl=/c/tcl/lib
	$ make
	$ make install


Examples
=====

List devices:

    package require openal

    set devices [openal::list_devices]
    puts "We found:"
    foreach device $devices {
        puts $device
    }

List capture devices:

    package require openal

    set captures [openal::list_captures]
    puts "We found:"
    foreach device $captures {
        puts $device
    }

Cowork with [tclsndfile](https://github.com/ray2501/tclsndfile):

    # tclopenal example:
    #     Using openal and libsndfile to play wav/ogg file
    #

    package require sndfile
    package require openal

    set bits 16
    set channels 2
    set samplerate 44100
    set alformat AL_FORMAT_STEREO16

    if {$argc == 1} {
        set name [lindex $argv 0]
    } else {
        puts "Please give the correct argument!"
        exit
    }

    if {[catch {set data [sndfile snd0 $name READ]}]} {
        puts "Read file failed."
        exit
    } else {
        set encoding [dict get $data encoding]

        switch $encoding {
            {pcm_16} {
                    set bits 16
                }
                {pcm_24} {
                    set bits 24
                }
                {pcm_32} {
                    set bits 32
                }
                {pcm_s8} {
                    set bits 8
                }
                {pcm_u8} {
                    set bits 8
                }
                default {
                    set bits 16
                }
        }

        set channels [dict get $data channels]
        set samplerate [dict get $data samplerate]
        set size [expr [dict get $data frames] * $channels * $bits / 8]
        set buffersize [expr $samplerate * $bits / 8]
        snd0 buffersize $buffersize
        set buffer_number [expr $size / $buffersize + 1]
    }

    openal::device dev0
    dev0 setListener AL_POSITION [list 0 0 1.0]
    dev0 setListener AL_VELOCITY [list 0 0 0]
    dev0 setListener AL_ORIENTATION [list 0.0 0.0 1.0 0.0 1.0 0.0]
    dev0 createSource
    dev0 setSource AL_PITCH 1.0
    dev0 setSource AL_GAIN  1.0
    dev0 setSource AL_POSITION [list 0 0 1.0]
    dev0 setSource AL_VELOCITY [list 0 0 0]
    dev0 setSource AL_LOOPING 0

    if {$channels > 1} {
        if {$bits == 8} {
            set alformat AL_FORMAT_STEREO8
        } else {
            set alformat AL_FORMAT_STEREO16
        }
    } else {
        if {$bits == 8} {
            set alformat AL_FORMAT_MONO8
        } else {
            set alformat AL_FORMAT_MONO16
        }
    }

    dev0 createBuffer $buffer_number

    # problem: if read file failed, what should I do?
    for {set i 0} {$i < $buffer_number} {incr i} {
        if {[catch {set buffer [snd0 read_short]}] == 0} {
            dev0 bufferData $alformat $buffer $samplerate $i
            dev0 setSource AL_BUFFER $i
            dev0 playSource

            set state [dev0 getSource AL_SOURCE_STATE]
            while {[string compare $state "AL_PLAYING"]==0} {
                set state [dev0 getSource AL_SOURCE_STATE]
            }
        }
    }

    snd0 close

    dev0 destroySource
    dev0 destroyBuffer
    dev0 close

Cowork with [tclmpg123](https://github.com/ray2501/tclmpg123):

    # tclopenal example:
    #     Using openal and libmpg123 to play mp3 file
    #

    package require mpg123
    package require openal

    set bits 16
    set channels 2
    set samplerate 44100
    set alformat AL_FORMAT_STEREO16

    if {$argc == 1} {
        set name [lindex $argv 0]
    } else {
        puts "Please give the correct argument!"
        exit
    }

    if {[catch {set data [mpg123 mpg0 $name]}]} {
        puts "Read file failed."
        exit
    } else {
        set bits [dict get $data bits]
        set channels [dict get $data channels]
        set samplerate [dict get $data samplerate]
        set size [expr [dict get $data length] * $channels * $bits / 8]
        set buffersize 882200
        set buffersize [expr $samplerate * $bits / 8]
        mpg0 buffersize $buffersize
        set buffer_number [expr $size / $buffersize + 1]
    }

    openal::device dev0
    dev0 setListener AL_POSITION [list 0 0 1.0]
    dev0 setListener AL_VELOCITY [list 0 0 0]
    dev0 setListener AL_ORIENTATION [list 0.0 0.0 1.0 0.0 1.0 0.0]
    dev0 createSource
    dev0 setSource AL_PITCH 1.0
    dev0 setSource AL_GAIN  1.0
    dev0 setSource AL_POSITION [list 0 0 1.0]
    dev0 setSource AL_VELOCITY [list 0 0 0]
    dev0 setSource AL_LOOPING 0

    if {$channels > 1} {
        if {$bits == 8} {
            set alformat AL_FORMAT_STEREO8
        } else {
            set alformat AL_FORMAT_STEREO16
        }
    } else {
        if {$bits == 8} {
            set alformat AL_FORMAT_MONO8
        } else {
            set alformat AL_FORMAT_MONO16
        }
    }

    # Load file data to our buffers
    dev0 createBuffer $buffer_number
    set buffer_index 0
    while {$buffer_index < $buffer_number} {
        if {[catch {set buffer [mpg0 read]}] == 0} {
            dev0 bufferData $alformat $buffer $samplerate $buffer_index
        } else {
            break
        }

        incr buffer_index
    }

    dev0 queueBuffers $buffer_index

    # Just for test, get the total number of queued buffers
    set buffer_index [dev0 getSource AL_BUFFERS_QUEUED]

    set val 0
    while {$val < $buffer_index} {
        dev0 playSource

        set state [dev0 getSource AL_SOURCE_STATE]
        while {[string compare $state "AL_PLAYING"]==0} {
            set state [dev0 getSource AL_SOURCE_STATE]
        }
        set val [dev0 getSource AL_BUFFERS_PROCESSED]
    }

    dev0 unqueueBuffers $buffer_index
    mpg0 close

    dev0 destroySource
    dev0 destroyBuffer
    dev0 close

Capture (only for function test)

    package require openal
    package require sndfile

    set savename [clock format [clock seconds] -format %Y%m%d]
    append savename ".wav"

    sndfile snd1 $savename WRITE -rate 44100 -channels 1 \
      -fileformat wav -encoding pcm_16

    openal::capture cap0 -rate 44100 -format AL_FORMAT_MONO16 -buffersize 88200
    cap0 start

    after 500
    for {set i 0} {$i < 60} {incr i} {
        set buffer [cap0 captureSamples]
        if {[string length $buffer] > 0} {
            snd1 write_short $buffer
        } else {
            after 500
        }
    }

    cap0 stop

    snd1 close
    cap0 close

