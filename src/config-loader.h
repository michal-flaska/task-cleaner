#pragma once
#include <string>
#include <vector>

struct Config {
	std::vector<std::string> processes{};
	int check_interval_ms = 1000;
};

class ConfigLoader {
public:
	static Config Load(const std::string& filepath);
};
