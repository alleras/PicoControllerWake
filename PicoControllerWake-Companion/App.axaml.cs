using System;
using Avalonia;
using Avalonia.Controls.ApplicationLifetimes;
using Avalonia.Data.Core.Plugins;
using System.Linq;
using Avalonia.Controls.Notifications;
using Avalonia.Markup.Xaml;
using Avalonia.Threading;
using Microsoft.Extensions.DependencyInjection;
using PicoControllerWake_Companion.Extensions;
using PicoControllerWake_Companion.ViewModels;
using PicoControllerWake_Companion.Views;

namespace PicoControllerWake_Companion;

public partial class App : Application
{
    public override void Initialize()
    {
        AvaloniaXamlLoader.Load(this);
    }

    public override void OnFrameworkInitializationCompleted()
    {
        if (ApplicationLifetime is IClassicDesktopStyleApplicationLifetime desktop)
        {
            // Avoid duplicate validations from both Avalonia and the CommunityToolkit. 
            // More info: https://docs.avaloniaui.net/docs/guides/development-guides/data-validation#manage-validationplugins
            DisableAvaloniaDataAnnotationValidation();

            var collection = new ServiceCollection();
            collection.AddCommonServices();
            
            var services = collection.BuildServiceProvider();
            var viewModel = services.GetRequiredService<MainWindowViewModel>();
            
            desktop.MainWindow = new MainWindow
            {
                DataContext = viewModel
            };
            
            ToastNotifications.Initialize(desktop.MainWindow);
        }
        
        Dispatcher.UIThread.UnhandledException += (s, e) =>
        {
            e.Handled = true;
            Console.WriteLine(e.Exception);
            ToastNotifications.Show(e.Exception.Message, NotificationType.Error);
        };

        base.OnFrameworkInitializationCompleted();
    }

    private void DisableAvaloniaDataAnnotationValidation()
    {
        // Get an array of plugins to remove
        var dataValidationPluginsToRemove =
            BindingPlugins.DataValidators.OfType<DataAnnotationsValidationPlugin>().ToArray();

        // remove each entry found
        foreach (var plugin in dataValidationPluginsToRemove)
        {
            BindingPlugins.DataValidators.Remove(plugin);
        }
    }
}