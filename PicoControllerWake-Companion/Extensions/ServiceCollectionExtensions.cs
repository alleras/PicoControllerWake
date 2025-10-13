using Microsoft.Extensions.DependencyInjection;
using PicoControllerWake_Companion.Interfaces;
using PicoControllerWake_Companion.Services;
using PicoControllerWake_Companion.ViewModels;

namespace PicoControllerWake_Companion.Extensions;

public static class ServiceCollectionExtensions
{
    public static void AddCommonServices(this IServiceCollection services)
    {
        services.AddSingleton<IWakeBridgeService, WakeBridgeService>();
        services.AddTransient<IBluetoothService, BluetoothService>();
        services.AddTransient<MainWindowViewModel>();
    }
}