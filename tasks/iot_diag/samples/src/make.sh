#!/bin/bash

# pip install platformio
mkdir iot_diag && cd iot_diag
pio project init --board nucleo_f103rb --project-option "framework=stm32cube"
mkdir src
cp ../platformio.ini .
cp -r ../main.c src/
pio run
cd ..
cp iot_diag/.pio/build/nucleo_f103rb/firmware.bin iot_diag.bin
rm -rf iot_diag
