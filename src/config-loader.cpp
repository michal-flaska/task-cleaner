#include "config-loader.h"
#include "../libs/json.hpp"
#include <fstream>
#include <stdexcept>

using json = nlohmann::json;

Config ConfigLoader::Load(const std::string& filepath) {
	std::ifstream file(filepath);
	if (!file.is_open()) {
		throw std::runtime_error("Failed to open config file: " + filepath);
	}

	json j;
	file >> j;

	Config config;
	config.processes = j["processes"].get<std::vector<std::string>>();
	config.check_interval_ms = j["check_interval_ms"].get<int>();

	return config;
}
