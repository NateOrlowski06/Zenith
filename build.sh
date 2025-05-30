#! /home/nate/Programming/Zenith   
#path to your project directory
export PICO_SDK_PATH=~/pico/pico-sdk
rm -rf build
mkdir build
cd build
cmake ..
make
