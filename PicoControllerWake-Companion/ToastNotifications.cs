using System;
using Avalonia.Controls;
using Avalonia.Controls.Notifications;

namespace PicoControllerWake_Companion;

public static class ToastNotifications
{
    private static WindowNotificationManager? _notificationManager;

    public static void Initialize(Window mainWindow)
    {
        _notificationManager = new WindowNotificationManager(mainWindow)
        {
            Position = NotificationPosition.BottomRight,
            MaxItems = 2,
        };
    }

    public static void Show(string message, NotificationType type = NotificationType.Information)
    {
        _notificationManager?.Show(new Notification(type.ToString(), message, type, TimeSpan.FromSeconds(3)));
    }
}
