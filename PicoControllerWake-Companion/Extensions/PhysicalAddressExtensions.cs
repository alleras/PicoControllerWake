using System.Linq;
using System.Net.NetworkInformation;

namespace PicoControllerWake_Companion.Extensions;

public static class PhysicalAddressExtensions
{
    public static string ToFormattedMacAddress(this PhysicalAddress address)
    {
        return string.Join(":", address.GetAddressBytes().Select(b => b.ToString("X2")));
    }
}