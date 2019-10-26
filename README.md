# neopixel
An ESP IDF based neopixel implementation for ws6812 led strips.
There is a standalone executable that automatically starts with a preset effect. The effects can be switched at runtime via simple text commands delivered over serial line. RMT is used to drive the leds instead of bit banging to deliver flicker free effects.

A former version used wifi or bluetooth to receive the commands but since both of these are heavily active at random times, even the RMT was not able to deliver flicker free experience. The command system was therefore reimplemented to use UART and since then, no flickering was experienced. I will write a separate wifi to serial based translator that can run on a peer ESP32 connected via serial to the main unit so that a true wireless solution can be achieved.

## Serial connection from PC
Connect the unit using an FTDI USB to Serial board, or any other serial port emulation or a real serial port
- ESP TX GPIO4 : RX on FTDI
- ESP RX GPIO5 : TX on FTDI
- ESP GND : GND on FTDI

Run a terminal emulation such as putty and connect to the virtual serial port created by the FTDI. Alternatively you can use:

`idf.py monitor -p [port]`

Be sure to use the port registered for the ftdi and not the port used for flashing.
## Command syntax:

`[command]:[subcommand]:[param1];[param2];....`
sample:
`effect:police:150;10;128`

## Current commands:
- `effect:[effect name]:[effect params]`  
	all effects have some default parameters if parematers are omitted
	- `effect:solid` | `effect:solid:[led count];[iteration delay ms];[red];[green];[blue];[white]`  
	Set the strip to a fixed color.
	- `effect:stars` | `effect:stars:[led count];[iteration delay ms];[new star probability x/100 000];[dimming steps count];[red];[green];[blue];[white]`  
	Randomly shows fading stars. In each iteration all pixels are looped over. For a new star to occur at the given pixel a random number 0-100 000 is generated and compared to new star probability param. If higher a new star with the red, green, blue, white values is created. If the random number is lower the brightness of each channels is gradually decreased over dimming steps count iterations.  
	- `effect:police` | `effect:police:[led count];[iteration delay ms];[brightness]`  
	a red and blue cycle to light up your neigborhood and scare the criminals
	- `effect:rainbow` | `effect:rainbow:[led count];[iteration delay ms];[brightness]`  
	a color spectrum rainbow effect moving over the strip
	- `effect:snake` | `effect:snake:[led count];[iteration delay ms];[snake length];[dimmed tail length];[brightness]`  
	a fixed color snake goes there and back again over the led strip

## Custom effects
New effects can be written by sublcasing the effect class and overriding the virtual Run method. Note that the run method is supposed to do a single frame of the effect and return.  

To support commands for new effects, modify the effectFactory class that parses parameters and creates effect instances at runtime. It is the brains behind effect command parsing.

## Use as a library
The neopixel class, base effect class and its derivates can be also used as libraries in other projects.

## To build:
- Setup the ESP IDF toolchain according to: https://docs.espressif.com/projects/esp-idf/en/latest/get-started/linux-setup.html 
  - The project uses the CMake build system of ESP IDF and is being developed against the v4.1 of ESP IDF (bleeding edge)
- Clone [mirronelli\esp_cpp_components](https://github.com/mirronelli/esp_cpp_components) to a sibling folder.
- Clone this repo
- Execute ```idf.py build``` in the neopixel repo

## To change effect:
- first prepare the topic
  - use ```idf.py menuconfig```
  - in main project configuration change the mqtt broker address to point to your local mqtt server
  - set the command topic name that the ESP32 will listen to
  - save menuconfig and rebuild
- send an mqtt message to trigger the change
``` mosquitto_pub -t YOURTOPIC -m "effect:snake:"```
- future versions will parse parameters and pass those to effects


### Build comments
The project was written an built against esp-idf commit:
02c7c3885e10c75772a3d0984c1cdbb6ad9484f3
