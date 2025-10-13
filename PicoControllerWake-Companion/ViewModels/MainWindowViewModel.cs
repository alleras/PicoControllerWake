using System;
using System.Collections.ObjectModel;
using System.Linq;
using System.Net.NetworkInformation;
using System.Threading.Tasks;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using PicoControllerWake_Companion.Enums;
using PicoControllerWake_Companion.Extensions;
using PicoControllerWake_Companion.Interfaces;
using PicoControllerWake_Companion.Models;
using PicoControllerWake_Companion.Services;

namespace PicoControllerWake_Companion.ViewModels;

public partial class MainWindowViewModel : ViewModelBase
{
    private readonly IWakeBridgeService _wakeBridgeService;
    private readonly IBluetoothService _bluetoothService;
    
    [ObservableProperty] 
    private string _macAddress = string.Empty;

    [ObservableProperty] private bool _initialized = false;
    public ObservableCollection<BluetoothDeviceViewModel> BluetoothDeviceViews { get; } = [];

    public MainWindowViewModel()
    {
        BluetoothDeviceViews.Add(new BluetoothDeviceViewModel());
        BluetoothDeviceViews.Add(new BluetoothDeviceViewModel());
        BluetoothDeviceViews.Add(new BluetoothDeviceViewModel());
    } // For design time

    public MainWindowViewModel(IWakeBridgeService wakeBridgeService, IBluetoothService bluetoothService)
    {
        _wakeBridgeService = wakeBridgeService;
        _bluetoothService = bluetoothService;

        _ = InitializeAsync();
    }

    private async Task InitializeAsync()
    {
        var wakeBridgeInitialized = false;
        SerialConnectionErrors? error = null; 
        try
        {
            (wakeBridgeInitialized, error) = await _wakeBridgeService.InitializeAsync();
        }
        catch (Exception ex)
        {
            Console.WriteLine(ex);
            return;
        }
        
        if (!wakeBridgeInitialized)
        {
            //TODO: report errors here
            return;
        }
        
        await RefreshDevicesAsync();
        Initialized = true;
    }
    
    private async Task RefreshDevicesAsync()
    {
        BluetoothDeviceViews.Clear();
        
        var whitelistedMacs = _wakeBridgeService.GetWhitelistedBluetoothMacs();
        var btDevices = await _bluetoothService.GetPairedDevicesAsync();
        
        btDevices.ForEach(btDevice =>
        {
            var isWhiteListed = whitelistedMacs.Any(m => m.Equals(btDevice.MacAddress));
            BluetoothDeviceViews.Add(new BluetoothDeviceViewModel(
                btDevice, 
                isWhiteListed, 
                _wakeBridgeService));
        });
    }

    [RelayCommand]
    private async Task ClearDevices()
    {
        await _wakeBridgeService.ClearWhitelistedDevicesAsync();
        await RefreshDevicesAsync();
    }

    [RelayCommand]
    private async Task Reload()
    {
        await InitializeAsync();
    }
}