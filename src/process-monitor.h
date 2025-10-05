#pragma once
#include <string>
#include <vector>
#include <unordered_set>

class ProcessMonitor {
public:
	ProcessMonitor(const std::vector<std::string>& process_names);
	void ScanAndKill();

private:
	std::unordered_set<std::string> target_processes;
	void KillProcess(unsigned long process_id, const std::string& name);
	std::string ToLower(const std::string& str);
};
