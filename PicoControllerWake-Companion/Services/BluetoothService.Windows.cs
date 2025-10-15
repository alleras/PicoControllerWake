using System;
using System.Collections.Generic;
using System.Management;
using System.Net.NetworkInformation;
using System.Runtime.Versioning;
using System.Threading.Tasks;
using PicoControllerWake_Companion.Models;

namespace PicoControllerWake_Companion.Services;
public partial class BluetoothService
{
    private static bool IsWindowsBluetoothDeviceIdentifier(string identifier)
    {
        return identifier.StartsWith("BTHENUM\\DEV") || identifier.StartsWith("BTHLE\\DEV");
    }

    private static PhysicalAddress ExtractMacAddress(string deviceIdentifier)
    {
        // Get the second section of a string like "BTHLE\DEV_macaddresshere\..."
        var extractedId = deviceIdentifier
            .Split("\\")[1]
            .Replace("DEV_", string.Empty, StringComparison.InvariantCulture);
        
        return PhysicalAddress.Parse(extractedId);
    }
    
    /// <summary>
    /// Hacky implementation to get paired Bluetooth devices on Windows. Seems to work though!
    /// </summary>
    /// <returns></returns>
    [SupportedOSPlatform("windows")]
    private static Task<List<BluetoothDevice>> GetPairedDevicesWindows()
    {
        var devices = new List<BluetoothDevice>();

        using var searcher = new ManagementObjectSearcher("Select * from Win32_PnPEntity");
        foreach (var device in searcher.Get())
        {
            var name = device["Name"]?.ToString() ?? "";
            var id = device["DeviceID"]?.ToString() ?? "";

            if (IsWindowsBluetoothDeviceIdentifier(id))
            {
                devices.Add(new BluetoothDevice(name, ExtractMacAddress(id)));
            }
        }

        return Task.FromResult(devices);
    }
}