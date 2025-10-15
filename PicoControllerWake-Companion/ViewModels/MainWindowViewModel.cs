using System;
using System.Collections.ObjectModel;
using System.Linq;
using System.Threading.Tasks;
using Avalonia.Controls.Notifications;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using PicoControllerWake_Companion.Interfaces;

namespace PicoControllerWake_Companion.ViewModels;

public partial class MainWindowViewModel : ViewModelBase
{
    private readonly IWakeBridgeService _wakeBridgeService;
    private readonly IBluetoothService _bluetoothService;
    
    [ObservableProperty] 
    private string _macAddress = string.Empty;

    [ObservableProperty] private bool _initialized = false;
    public ObservableCollection<BluetoothDeviceViewModel> BluetoothDeviceViews { get; } = [];

#pragma warning disable CS8618 // Non-nullable field must contain a non-null value when exiting constructor. Consider adding the 'required' modifier or declaring as nullable.
    public MainWindowViewModel()
#pragma warning restore CS8618 // Non-nullable field must contain a non-null value when exiting constructor. Consider adding the 'required' modifier or declaring as nullable.
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
        var (wakeBridgeInitialized, error) = await _wakeBridgeService.InitializeAsync();
        
        if (!wakeBridgeInitialized && error is not null)
        {
            ToastNotifications.Show(error,  NotificationType.Error);
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