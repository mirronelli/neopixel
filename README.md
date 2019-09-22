# neopixel
An esp-idf based ws6812 neopixel implementation using rmt with a library of effects.

## To build:
- Setup the ESP IDF toolchain according to: https://docs.espressif.com/projects/esp-idf/en/latest/get-started/linux-setup.html 
  - The project uses the CMake build system of ESP IDF and is being developed against the v4.1 of ESP IDF (bleeding edge)
- Clone [mirronelli\esp_cpp_components](https://github.com/mirronelli/esp_cpp_components) to a sibling folder.
- Clone this repo
- Execute ```idf.py build``` in the neopixel repo
