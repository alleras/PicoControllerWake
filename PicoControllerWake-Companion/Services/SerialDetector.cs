using System;
using System.Linq;
using HidSharp;

namespace PicoControllerWake_Companion.Services;

public static partial class SerialDetector
{
    public static SerialDevice? FindSerialDevice(string productName)
    {
        // Find device using HidSharp
        var deviceList = DeviceList.Local;
        var devices = deviceList.GetSerialDevices();
        
        var picoDevice = devices
            .FirstOrDefault(s => s.DevicePath.Contains(productName, StringComparison.OrdinalIgnoreCase));
        
        return picoDevice;
    }
    public static string? GetPortName(SerialDevice device)
    {
        var devicePath = device.GetFileSystemName();
        
        // Windows: extract COM port
        if (devicePath.Contains("COM"))
        {
            var match = ComPortRegex().Match(devicePath);
            return match.Success ? match.Value : null;
        }

        return devicePath.StartsWith("/dev/") ? devicePath : null;
    }

    [System.Text.RegularExpressions.GeneratedRegex(@"COM\d+")]
    private static partial System.Text.RegularExpressions.Regex ComPortRegex();
}