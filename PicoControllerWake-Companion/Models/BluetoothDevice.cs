using System.Net.NetworkInformation;

namespace PicoControllerWake_Companion.Models;

public class BluetoothDevice
{
    public string Name { get; set; }
    public PhysicalAddress MacAddress { get; private set; }

    public BluetoothDevice(string name, PhysicalAddress macAddress)
    {
        Name = name;
        MacAddress = macAddress;
    }
}