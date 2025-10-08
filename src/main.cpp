#include "config-loader.h"
#include "process-monitor.h"
#include <windows.h>
#include <iostream>
#include <thread>
#include <chrono>

bool IsRegisteredInStartup() {
	HKEY hKey;
	if (RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_READ, &hKey) != ERROR_SUCCESS) {
		return false;
	}

	char value[MAX_PATH];
	DWORD size = sizeof(value);
	bool exists = RegQueryValueExA(hKey, "TaskCleaner", NULL, NULL, (LPBYTE)value, &size) == ERROR_SUCCESS;
	RegCloseKey(hKey);
	return exists;
}

bool RegisterStartup() {
	char exePath[MAX_PATH];
	if (!GetModuleFileNameA(NULL, exePath, MAX_PATH)) {
		return false;
	}

	HKEY hKey;
	if (RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_WRITE, &hKey) != ERROR_SUCCESS) {
		return false;
	}

	LONG result = RegSetValueExA(hKey, "TaskCleaner", 0, REG_SZ, (LPBYTE)exePath, (DWORD)strlen(exePath) + 1);
	RegCloseKey(hKey);
	return result == ERROR_SUCCESS;
}

void HideConsoleWindow() {
	HWND hwnd = GetConsoleWindow();
	if (hwnd != NULL) {
		ShowWindow(hwnd, SW_HIDE);
	}
}

int main() {
	// Register startup FIRST before hiding window
	if (!IsRegisteredInStartup()) {
		if (!RegisterStartup()) {
			MessageBoxA(NULL, "Failed to register startup. Run as Administrator.", "Task Cleaner", MB_ICONWARNING);
		}
	}

	// Now hide the window
	HideConsoleWindow();

	try {
		Config config = ConfigLoader::Load("config.json");
		ProcessMonitor monitor(config.processes);

		while (true) {
			monitor.ScanAndKill();
			std::this_thread::sleep_for(std::chrono::milliseconds(config.check_interval_ms));
		}
	}
	catch (const std::exception& e) {
		MessageBoxA(NULL, e.what(), "Task Cleaner Error", MB_ICONERROR);
		return 1;
	}

	return 0;
}
