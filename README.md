# PicoControllerWake
A Bluetooth PC waking system that works on the inexpensive Raspberry Pi Pico 2 W, to automatically wake your computer when whitelisted Bluetooth devices (like game controllers) broadcast advertisement packets. Used to bring PC gaming a bit closer to the console experience - just pick up your controller, turn it on, and go.

A quick [Google](https://www.google.com/search?q=turn+on+pc+with+xbox+controller&oq=turn+on+pc+with+xbox+controller) of a solution to this problem yields pretty disappointing results. Historically, only one dongle (model 1713) for Xbox controllers has supported this, and my experience with other controllers has been that they don't reliably wake a computer when the controller is connected/buttons are pressed. 

## How does it work?
Consoles typically rely on a separate chip that remains powered in standby mode, which listens for wake signals from paired controllers and powers on the console.

We don't quite have this functionality in the PC world, but the Pico 2 W has a Bluetooth chip that offers low-level detection capabilities. If we make it so that the Pico is always listening for [Bluetooth advertisement packets](https://en.wikipedia.org/wiki/Bluetooth_advertising), we can detect an advertisement from a controller we trust, and send a signal to the computer to wake up. Controllers will advertise as soon as they turn on so that they connect quickly, so this fits perfectly into what we want.

Most motherboards can be configured so that they keep USB devices powered on when asleep, and they will allow something like a keyboard to wake the computer up on a keypress. The firmware of the Pico is thus configured to emulate a HID device, so that we "press a key" to wake the computer up when we detect the controller.

## Requirements

- Raspberry Pi Pico 2 W (requires the wireless chip for Bluetooth scanning)
- Micro USB cable for power and communication with PC
- A computer with the following BIOS configuration (these options vary by manufacturer):
  - Keep USB powered on while asleep and
  - Enable wake on USB device

## How to Use
The firmware is provided for the Pico, and a companion app is provided to configure a whitelist of devices that it will accept for waking up:

1. Enter `BOOTSEL` mode on your Raspberry Pi Pico by pressing the "BOOTSEL" button and connecting the cable.
2. Drag and drop the .uf2 file from the [Releases Page](https://github.com/alleras/PicoControllerWake/releases)
3. Wait for the Pico to reset, then run the PicoControllerWake Companion app to configure the allowed Bluetooth devices.
4. The companion application will show you all paired devices to your computer. Simply check "Allow wake" on one to whitelist it.
5. Try turning on the controller/device while your computer is asleep. It should wake it up.
6. If you encounter problems, let me know in the [issues page](https://github.com/alleras/PicoControllerWake/issues)

## Screenshots
<img width="402" height="491" alt="image" src="https://github.com/user-attachments/assets/786a744d-e8ae-47f5-a18c-6538f4b22e98" />

## Current Limitations/To-do list
1. Only Bluetooth BLE is detected by the Pico (this means controllers that rely on Bluetooth Classic won't work just yet) (https://github.com/alleras/PicoControllerWake/issues/3)
2. The Pico device can only wake computers that put the USB Interface in suspended mode (https://github.com/alleras/PicoControllerWake/issues/2)
3. The companion App is unsigned, so it requires going through the SmartScreen window on Windows
4. Linux support has not been tested (https://github.com/alleras/PicoControllerWake/issues/4, https://github.com/alleras/PicoControllerWake/issues/5)
5. The companion app still has the default Avalonia Icon :) 
