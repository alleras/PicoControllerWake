## Building

### Software Requirements

- Pico SDK
- CMake 3.13 or higher

### Build Instructions

1. Set up your development environment. Follow [this guide](https://www.raspberrypi.com/documentation/microcontrollers/c_sdk.html) or install the [VSCode Extension](https://github.com/raspberrypi/pico-vscode).
   
2. Clone the repository:
```bash
git clone https://github.com/alleras/PicoControllerWake.git
```

3. Compile the project using the VSCode extension: `Command Palette` -> `Raspberry Pi Pico: Compile Pico Project`

4. Flash the generated `PicoControllerWake.uf2` file to your Pico 2 W by:
   - Holding the BOOTSEL button while plugging in the USB cable
   - Copying the .uf2 file to the mounted RPI-RP2 drive

## How it Works

### Device Emulation
The USB Descriptor of the Pico is configured to expose an HID and a CDC interface. The HID device is configured as a suspendable device with the ability to wake the computer. The CDC interface is exposed to enable communications via a Serial port, to communicate changes from the companion app.

**Files to check**: `btstack_config.h`, `usb_descriptors.c`, `tusb_config.h`, `config.h`, `CMakeLists.txt`

### Bluetooth Scanning
The device constantly scans for any Bluetooth devices that are advertising an `ADV_IND` packet. We use BTStack to enable low level scanning of Bluetooth packets.

**Files to check**: `bt_scanner.c`

### Whitelisting
Whitelisting is enabled via a `Whitelist` class tha handles `bluetooth-whitelist` commands and saves information accordingly to the Pico's flash.

**Files to check**: `flash_storage.c`, `whitelist.c`

### Main Loop
On startup, the Pi Pico loads the whitelist from flash, and begins scanning for Bluetooth Classic and BLE packets. A callback is registered to handle the scenario where a Whitelisted device's advertisement packets are detected.

**Files to check**: `PicoControllerWake.c`, `util.c`, `cdc_output.c`

### Waking
We use the TinyUSB HID functionality to wake the computer in the `PCWaker` class, via the `tud_remote_wakeup()` function. A space keypress is also sent as a fallback.

Wake actions are debounced in the `PCWaker` class to not spam actions to the computer.

**Files to check**: `pc_waker.c`
