# neopixel
An ESP IDF based neopixel implementation for ws6812 led stripes using RMT with a library of effects.
The project is a standalone executable that drives neopixel leds using the RMT. The effects are parametric.
It listens to mqtt commands to run the effects.

The neopixel class, base effect class and its derivates can be also used as libraries in other projects.

## To build:
- Setup the ESP IDF toolchain according to: https://docs.espressif.com/projects/esp-idf/en/latest/get-started/linux-setup.html 
  - The project uses the CMake build system of ESP IDF and is being developed against the v4.1 of ESP IDF (bleeding edge)
- Clone [mirronelli\esp_cpp_components](https://github.com/mirronelli/esp_cpp_components) to a sibling folder.
- Clone this repo
- Execute ```idf.py build``` in the neopixel repo
