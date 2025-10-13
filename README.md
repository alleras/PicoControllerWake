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
