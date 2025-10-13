using System;
using System.Collections.Generic;
using System.Threading.Tasks;
using PicoControllerWake_Companion.Interfaces;
using System.Runtime.InteropServices;
using PicoControllerWake_Companion.Models;

namespace PicoControllerWake_Companion.Services;

public partial class BluetoothService : IBluetoothService
{
    public async Task<List<BluetoothDevice>> GetPairedDevicesAsync()
    {
        if (RuntimeInformation.IsOSPlatform(OSPlatform.Windows))
        {
            return await GetPairedDevicesWindows();
        }
        if (RuntimeInformation.IsOSPlatform(OSPlatform.OSX))
        {
            return await GetPairedDevicesMacOS();
        }
        if (RuntimeInformation.IsOSPlatform(OSPlatform.Linux))
        {
            return await GetPairedDevicesLinux();
        }
        
        throw new InvalidOperationException("Platform not supported");
    }
}