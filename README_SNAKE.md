# Hello!

REDO this readme with...
- Setup for VSCode
- Setup for workspace with better setup stuffs, and put pico-sdk in the build folder? idk :D

# Setup
- git clone git@github.com:raspberrypi/pico-sdk.git
- git clone --recursive git@github.com:raspberrypi/pico-sdk.git (if you want tinyUSB and whatever else)
- export PICO_SDK_PATH=../../pico-sdk ? Only if not using VSCode?
- cd pico-sdk
- git submodule update --init
- bruh idk bit it works now....

# Building Without VSCode
From the workspace directory:
- mkdir build
- cd build
- cmake ..
- Add ${workspaceFolder}/build/pico-sdk/** to includePath
- make -j4
- cp snakeBox.uf2 /media/douglas/RPI-RP2/

# Building With VSCode
- Get the goodies
- Press Use CMake tab to configure or build projects
- deploy by cp uf2 to the /media/whatever

# Debug
- minicome -b 115200 -o -D /dev/ttyACM0

Part List and links
- Pi PICO
- the Ultrasonic Sensor boi
- Servo and horn


pico-sdk reading
- https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf (Getting Started)
- https://datasheets.raspberrypi.com/pico/raspberry-pi-pico-c-sdk.pdf (More in depth)