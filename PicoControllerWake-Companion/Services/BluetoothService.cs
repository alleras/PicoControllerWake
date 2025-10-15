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
        if (OperatingSystem.IsWindows())
        {
            return await GetPairedDevicesWindows();
        }
        if (OperatingSystem.IsMacOS())
        {
            return await GetPairedDevicesMacOS();
        }
        if (OperatingSystem.IsLinux())
        {
            return await GetPairedDevicesLinux();
        }
        
        throw new InvalidOperationException("Platform not supported");
    }
}