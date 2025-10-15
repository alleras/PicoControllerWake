using System.Collections.Generic;
using System.Net.NetworkInformation;
using System.Threading.Tasks;

namespace PicoControllerWake_Companion.Interfaces;

public interface IWakeBridgeService
{
    Task<(bool, string?)> InitializeAsync();
    Task AddWhitelistedDeviceAsync(PhysicalAddress mac);
    Task RemoveWhitelistedDeviceAsync(PhysicalAddress mac);
    Task ClearWhitelistedDevicesAsync();
    IEnumerable<PhysicalAddress> GetWhitelistedBluetoothMacs();
}