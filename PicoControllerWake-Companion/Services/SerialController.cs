using System;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PicoControllerWake_Companion.Services;

public class SerialController : IDisposable
{
    private SerialPort? _serialPort;
    
    public async Task<bool> ConnectAsync(string portName, int baudRate = 9600)
    {
        _serialPort = new SerialPort(portName, baudRate)
        {
            ReadTimeout = 5000,
            WriteTimeout = 5000,
            DtrEnable = true,
            RtsEnable = true,
            NewLine = "\n"
        };
        _serialPort.Open();

        await Task.Delay(200);
        _serialPort.DiscardInBuffer();
        
        Console.WriteLine($"Connected to Pico on {portName}");
        return true;
    }
    
    public string? SendCommand(string command)
    {
        if (_serialPort is not { IsOpen: true }) return null;
        
        _serialPort.DiscardInBuffer();
        _serialPort.WriteLine(command);
        
        var response = new StringBuilder();

        while (_serialPort.ReadLine() is { } line)
        {
            if (line.Trim() == "OK|EOF")
                break;
                
            response.AppendLine(line);
        }
        
        var result = response.ToString().Trim();
        Console.WriteLine(result);

        return StripDebugLines(result);
    }

    private static string StripDebugLines(string serialCommandResponse)
    {
        var lines = serialCommandResponse
            .Split('\n', StringSplitOptions.None)
            .Where(line => !line.StartsWith("DEBUG|"))
            .ToArray();

        return string.Join("\n", lines);
    }
    
    public void Dispose()
    {
        GC.SuppressFinalize(this);
        if (_serialPort is not null && _serialPort.IsOpen)
        {
            _serialPort?.Close();
        }
        _serialPort?.Dispose();
    }
}