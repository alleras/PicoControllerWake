using System;
using System.Collections.Generic;
using System.Runtime.Versioning;
using System.Threading.Tasks;
using PicoControllerWake_Companion.Models;

namespace PicoControllerWake_Companion.Services;

public partial class BluetoothService
{
    [SupportedOSPlatform("linux")]
    private static Task<List<BluetoothDevice>> GetPairedDevicesLinux()
    {
        throw new NotImplementedException();
    }
}