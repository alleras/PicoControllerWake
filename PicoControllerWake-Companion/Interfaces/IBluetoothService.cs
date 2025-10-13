using System.Collections.Generic;
using System.Threading.Tasks;
using PicoControllerWake_Companion.Models;

namespace PicoControllerWake_Companion.Interfaces;

public interface IBluetoothService
{
    Task<List<BluetoothDevice>> GetPairedDevicesAsync();
}