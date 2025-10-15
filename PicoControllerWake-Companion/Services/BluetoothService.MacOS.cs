using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Net.NetworkInformation;
using System.Runtime.Versioning;
using System.Text.Json;
using System.Threading.Tasks;
using PicoControllerWake_Companion.Models;

namespace PicoControllerWake_Companion.Services;

public partial class BluetoothService
{
    // ReSharper disable once InconsistentNaming
    [SupportedOSPlatform("macos")]
    private static async Task<List<BluetoothDevice>> GetPairedDevicesMacOS()
    {
        var process = new Process
        {
            StartInfo = new ProcessStartInfo
            {
                FileName = "system_profiler",
                Arguments = "SPBluetoothDataType -json",
                RedirectStandardOutput = true,
                UseShellExecute = false,
                CreateNoWindow = true
            }
        };
    
        process.Start();
        var output = await process.StandardOutput.ReadToEndAsync();
        await process.WaitForExitAsync();
        
        var json = JsonDocument.Parse(output);
        var bluetoothData = json.RootElement.GetProperty("SPBluetoothDataType")[0];

        var devices = new List<BluetoothDevice>();
        
        AddDevicesFromProperty(bluetoothData, "device_connected", devices);
        AddDevicesFromProperty(bluetoothData, "device_not_connected", devices);
    
        return devices;
    }

    private static void AddDevicesFromProperty(JsonElement bluetoothData, string propertyName, List<BluetoothDevice> devices)
    {
        if (!bluetoothData.TryGetProperty(propertyName, out var jsonDevicesList))
            return;

        /* TODO: Test in Mac OS
         
        var devicesList = jsonDevicesList
            .EnumerateArray()
            .SelectMany(arrayElement => arrayElement.EnumerateObject())
            .Select(deviceObj => new BluetoothDevice(
                deviceObj.Name,
                PhysicalAddress.Parse(deviceObj.Value.GetProperty("device_address").GetString()!)
            ));
        
        devices.AddRange(devicesList);
        */

        foreach (var arrayElement in jsonDevicesList.EnumerateArray())
        {
            foreach (var deviceObj in arrayElement.EnumerateObject())
            {
                var deviceName = deviceObj.Name;
                var deviceAddress = deviceObj.Value.GetProperty("device_address").GetString();
                
                devices.Add(new BluetoothDevice(deviceName, PhysicalAddress.Parse(deviceAddress)));
            }
        }
    }
}