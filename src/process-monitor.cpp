#include "process-monitor.h"
#include <windows.h>
#include <tlhelp32.h>
#include <algorithm>
#include <iostream>

ProcessMonitor::ProcessMonitor(const std::vector<std::string>& process_names) {
	for (const auto& name : process_names) {
		target_processes.insert(ToLower(name));
	}
}

void ProcessMonitor::ScanAndKill() {
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snapshot == INVALID_HANDLE_VALUE) return;

	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);

	if (Process32First(snapshot, &pe32)) {
		do {
			std::string proc_name = ToLower(pe32.szExeFile);
			if (target_processes.count(proc_name) > 0) {
				KillProcess(pe32.th32ProcessID, pe32.szExeFile);
			}
		} while (Process32Next(snapshot, &pe32));
	}

	CloseHandle(snapshot);
}

void ProcessMonitor::KillProcess(unsigned long process_id, const std::string& name) {
	HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, process_id);
	if (hProcess != NULL) {
		if (TerminateProcess(hProcess, 0)) {
			std::cout << "Killed: " << name << " (PID: " << process_id << ")" << std::endl;
		}
		CloseHandle(hProcess);
	}
}

std::string ProcessMonitor::ToLower(const std::string& str) {
	std::string result = str;
	std::transform(result.begin(), result.end(), result.begin(), ::tolower);
	return result;
}
