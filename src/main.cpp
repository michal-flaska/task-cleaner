#include "config-loader.h"
#include "process-monitor.h"
#include <windows.h>
#include <iostream>
#include <thread>
#include <chrono>

void HideConsoleWindow() {
	HWND hwnd = GetConsoleWindow();
	ShowWindow(hwnd, SW_HIDE);
}

bool IsRegisteredInStartup() {
	HKEY hKey;
	if (RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
		char value[MAX_PATH];
		DWORD size = sizeof(value);
		bool exists = RegQueryValueExA(hKey, "TaskCleaner", NULL, NULL, (LPBYTE)value, &size) == ERROR_SUCCESS;
		RegCloseKey(hKey);
		return exists;
	}
	return false;
}

void RegisterStartup() {
	char exePath[MAX_PATH];
	GetModuleFileNameA(NULL, exePath, MAX_PATH);

	HKEY hKey;
	if (RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_WRITE, &hKey) == ERROR_SUCCESS) {
		RegSetValueExA(hKey, "TaskCleaner", 0, REG_SZ, (LPBYTE)exePath, (DWORD)strlen(exePath) + 1);
		RegCloseKey(hKey);
	}
}

int main() {
	HideConsoleWindow();

	if (!IsRegisteredInStartup()) {
		RegisterStartup();
	}

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
