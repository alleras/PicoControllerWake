using System.Collections.Generic;
using System.Management;
using System.Net.NetworkInformation;
using System.Threading.Tasks;
using PicoControllerWake_Companion.Models;

namespace PicoControllerWake_Companion.Services;

public partial class BluetoothService
{
    private async Task<List<BluetoothDevice>> GetPairedDevicesWindows()
    {
        return await Task.Run(() =>
        {
            var devices = new List<BluetoothDevice>();
            
            using var searcher = new ManagementObjectSearcher(
                "SELECT * FROM Win32_PnPEntity WHERE PNPClass='Bluetooth' OR Service='BTHUSB'");
            
            foreach (ManagementObject device in searcher.Get())
            {
                var name = device["Name"]?.ToString();
                var deviceId = device["DeviceID"]?.ToString();

                if (string.IsNullOrEmpty(name) || string.IsNullOrEmpty(deviceId))
                    continue;

                // Extract MAC address from DeviceID (format: BTHENUM\{...}\{MAC_ADDRESS}&...)
                var macAddress = ExtractMacAddress(deviceId);
                
                if (macAddress != null)
                {
                    devices.Add(new BluetoothDevice(name, macAddress));
                }
            }

            return devices;
        });
    }

    private static PhysicalAddress? ExtractMacAddress(string deviceId)
    {
        try
        {
            // DeviceID format often contains MAC like: ...\{XX_XX_XX_XX_XX_XX}&...
            var parts = deviceId.Split('\\', '&');
            
            foreach (var part in parts)
            {
                // Look for pattern with underscores (12 hex chars with underscores)
                if (part.Length == 17 && part.Contains('_'))
                {
                    var macString = part.Replace("_", "");
                    if (macString.Length == 12)
                    {
                        return PhysicalAddress.Parse(macString);
                    }
                }
            }
        }
        catch
        {
            // Invalid format
        }

        return null;
    }
}