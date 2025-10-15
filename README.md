# PicoControllerWake

A Bluetooth PC wake system for Raspberry Pi Pico 2 W that automatically wakes your computer when whitelisted Bluetooth devices (like game controllers) broadcast connectable packets.

## Features

- Scans for both Classic Bluetooth and BLE devices
- Only responds to authorized devices, configured with the companion app
- Wakes PC by simulating keyboard input

## Hardware Requirements

- Raspberry Pi Pico 2 W (requires CYW43 wireless chip for Bluetooth)
- USB cable for power and communication with PC

## How to Use
1. Enter `BOOTSEL` mode on your Raspberry Pi Pico by pressing the "BOOTSEL" button and connecting the cable.
2. Drag and drop the .uf2 file in the [Releases Page](https://github.com/alleras/PicoControllerWake/releases)
3. Wait for the Pico to reset, then run the Companion app to configure the allowed Bluetooth devices. I only recommend using this with Controllers!
4. Try turning on the controller while your computer is asleep.
5. If you encounter issues, visit the Troubleshooting Guide or open an Issue

## Screenshots
<img width="402" height="491" alt="image" src="https://github.com/user-attachments/assets/786a744d-e8ae-47f5-a18c-6538f4b22e98" />

## Current Limitations/To-do list
1. Only Bluetooth BLE is detected by the Pico (https://github.com/alleras/PicoControllerWake/issues/3)
2. The Pico device can only wake computers that put the USB Interface in suspended mode (https://github.com/alleras/PicoControllerWake/issues/2)
3. The companion App is unsigned, so it requires going through the SmartScreen window on Windows
4. Linux support has not been tested (https://github.com/alleras/PicoControllerWake/issues/4, https://github.com/alleras/PicoControllerWake/issues/5)
5. The companion app still has the default Avalonia Icon :) 
