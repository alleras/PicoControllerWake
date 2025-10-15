using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.NetworkInformation;
using System.Threading.Tasks;
using PicoControllerWake_Companion.Constants;
using PicoControllerWake_Companion.Extensions;
using PicoControllerWake_Companion.Interfaces;

namespace PicoControllerWake_Companion.Services;

public class WakeBridgeService : IWakeBridgeService
{
    private readonly SerialController _serialController = new();
    private bool _isInitialized;

    public async Task<(bool, string?)> InitializeAsync()
    {
        if (_isInitialized) return(true, null);
        
        var pico = SerialDetector.FindSerialDevice("Pico Wake Bridge");
        if (pico is null)
        {
            return (false, SerialConnectionErrors.PicoWakeBridgeNotFound);
        }

        try
        {
            await _serialController.ConnectAsync(pico.GetFileSystemName());
        }
        catch(Exception ex)
        {
            return (false, $"Failed to connect to {pico.GetFileSystemName()}: {ex.Message}");
        }
        
        _isInitialized = true;
        return (true, null);
    }

    public IEnumerable<PhysicalAddress> GetWhitelistedBluetoothMacs()
    {
        var response = _serialController.SendCommand("bluetooth-whitelist --list");
        
        if (response is null || response.Contains("ERR|"))
        {
            throw new Exception("There was an error getting the device list");
        }

        return response.Split('\n').Skip(1)
            .Select(s => s.Trim())
            .Select(PhysicalAddress.Parse);
    }

    public Task AddWhitelistedDeviceAsync(PhysicalAddress mac)
    {
        var response = _serialController.SendCommand($"bluetooth-whitelist --add {mac.ToFormattedMacAddress()}");

        if (response is null || response.Contains("ERR|"))
        {
            throw new Exception("There was an error adding the device");
        }

        return Task.CompletedTask;
    }
    
    public Task RemoveWhitelistedDeviceAsync(PhysicalAddress mac)
    {
        var response = _serialController.SendCommand($"bluetooth-whitelist --remove {mac.ToFormattedMacAddress()}");

        if (response is null || response.Contains("ERR|"))
        {
            throw new Exception("There was an error adding the device");
        }

        return Task.CompletedTask;
    }

    public Task ClearWhitelistedDevicesAsync()
    {
        var response = _serialController.SendCommand("bluetooth-whitelist --clear");

        if (response is null || response.Contains("ERR|"))
        {
            throw new Exception("There was an error adding the device");
        }

        return Task.CompletedTask;
    }
}