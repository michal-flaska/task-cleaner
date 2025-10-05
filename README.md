![task cleaner banner](./artwork/task-cleaner-banner.png)

A lightweight Windows utility that automatically monitors and terminates specified processes. Perfect for keeping your system clean from unwanted applications that keep restarting.

> [!NOTE]
> This tool is for legitimate system administration purposes. Use responsibly and only on your own systems. Terminating system processes can cause instability or data loss.

## Features

- **Automatic Process Monitoring** - Continuously scans running processes at configurable intervals
- **JSON Configuration** - Easy-to-edit configuration file for managing target processes
- **Auto-Startup** - Registers itself to run on Windows startup automatically
- **Minimal Resource Usage** - Efficient monitoring with low CPU and memory footprint
- **Silent Operation** - Runs hidden in the background without interrupting your workflow

## Why Does Windows Show "Windows protected your PC"?

When you run Task Cleaner for the first time, Windows Defender SmartScreen may display a warning that says "Windows protected your PC". This happens because:

1. **Unsigned Executable** - The application isn't signed with a code signing certificate (which costs money and requires verification)
2. **Administrator Privileges** - Task Cleaner requires admin rights to terminate processes, which triggers security warnings
3. **Startup Registration** - The app modifies the Windows Registry to auto-start, a behavior common in malware (though legitimate software uses it too)
4. **Low Reputation** - New executables without an established download history are flagged as potentially unsafe

**This is completely normal for independent software.** If you compiled it yourself from this source code, you know exactly what it does. To bypass the warning, click "More info" then "Run anyway".

## Configuration

Edit `config.json` to customize which processes to monitor:

```json
{
  "processes": [
    "CalculatorApp.exe",
    "notepad.exe"
  ],
  "check_interval_ms": 1000
}
```

- **processes** - List of executable names to terminate (case-insensitive)
- **check_interval_ms** - How often to scan for processes (in milliseconds)

## Building from Source

### Requirements

- Visual Studio 2022 (or newer)
- Windows SDK 10.0
- C++20 support

### Steps

1. Clone the repository:
   ```bash
   git clone https://github.com/michal-flaska/task-cleaner.git
   cd task-cleaner
   ```
2. Open `task-cleaner.sln` in Visual Studio
3. Build the solution (Ctrl+Shift+B or F7)
4. The executable will be in `x64/Release/task-cleaner.exe`

## Usage

1. Configure target processes in `config.json`
2. Run `task-cleaner.exe` as Administrator
3. The application will:
   - Hide its console window
   - Register itself for auto-startup (first run only)
   - Begin monitoring and terminating target processes

To stop the application, use Task Manager to end the `task-cleaner.exe` process.

## Technical Details

- **Language**: C++20
- **Platform**: Windows (uses Win32 API)
- **Dependencies**: nlohmann/json (header-only, included)
- **Privileges**: Requires Administrator rights to terminate processes

## Uninstalling

1. Kill the process via Task Manager (`task-cleaner.exe`)
2. Remove the registry entry:
   - Open Registry Editor (regedit)
   - Navigate to `HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run`
   - Delete the `TaskCleaner` entry
3. Delete the executable and config file
