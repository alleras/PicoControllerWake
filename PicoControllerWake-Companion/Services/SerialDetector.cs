using System;
using System.Linq;
using HidSharp;

namespace PicoControllerWake_Companion.Services;

public static class SerialDetector
{
    private static bool IsDeviceProductName(SerialDevice device, string productName)
    {
        return device.DevicePath.Contains(productName, StringComparison.OrdinalIgnoreCase) ||
               (OperatingSystem.IsWindows() && (
                   device.GetProductName().Contains(productName, StringComparison.OrdinalIgnoreCase)
               ));
    }
    
    public static SerialDevice? FindSerialDevice(string productName)
    {
        var deviceList = DeviceList.Local;
        var devices = deviceList.GetSerialDevices();
        
        return devices.FirstOrDefault(d => IsDeviceProductName(d, productName));;
    }
}