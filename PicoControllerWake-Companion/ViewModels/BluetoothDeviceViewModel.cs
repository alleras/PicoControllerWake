using System;
using System.Net.NetworkInformation;
using System.Threading.Tasks;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using PicoControllerWake_Companion.Extensions;
using PicoControllerWake_Companion.Interfaces;
using PicoControllerWake_Companion.Models;
using PicoControllerWake_Companion.Services;

namespace PicoControllerWake_Companion.ViewModels;

public partial class BluetoothDeviceViewModel : ViewModelBase
{
    private readonly PhysicalAddress _macAddress;
    private readonly IWakeBridgeService _wakeBridgeService;
    
    [ObservableProperty] private string _name;
    [ObservableProperty] private string _address;
    [ObservableProperty] private bool _isWhitelisted;

    public BluetoothDeviceViewModel()
    {
        _macAddress = PhysicalAddress.None;
        Name = "Test Device";
        Address = _macAddress.ToFormattedMacAddress();
    }

    public BluetoothDeviceViewModel(BluetoothDevice device, bool isWhiteListed, 
        IWakeBridgeService wakeBridgeService)
    {
        Name = device.Name;
        Address = device.MacAddress.ToFormattedMacAddress();
        IsWhitelisted = isWhiteListed;

        _macAddress = device.MacAddress;
        _wakeBridgeService = wakeBridgeService;
    }

    [RelayCommand]
    private async Task IsWhitelistedChanged()
    {
        if (IsWhitelisted)
        {
            await _wakeBridgeService.AddWhitelistedDeviceAsync(_macAddress);
        }
        else
        {
            await _wakeBridgeService.RemoveWhitelistedDeviceAsync(_macAddress);
        }
    }
}