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

int main() {
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
