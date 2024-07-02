#include "SimpleIni.h"

#include <fstream>
#include <sstream>
#include <iostream>

const std::string configFile = "config.ini";

SimpleIni ini(configFile);

SimpleIni::SimpleIni(const std::string& fileName)
{
	filename = fileName;
}

bool SimpleIni::read()
{
	std::ifstream file(filename);
	if (!file.is_open()) return false;

	std::string line;
	std::string currentSection;

    while (getline(file, line)) {
        // Skip comments and empty lines
        if (line.empty() || line[0] == ';' || line[0] == '#') continue;

        if (line[0] == '[') {
            size_t end = line.find(']');
            if (end != std::string::npos) {
                currentSection = line.substr(1, end - 1);
            }
            continue;
        }

        size_t equal = line.find('=');
        if (equal != std::string::npos) {
            std::string key = line.substr(0, equal);
            std::string value = line.substr(equal + 1);
            data[currentSection][key] = value;
        }
    }

    file.close();
    return true;
}

void SimpleIni::write()
{
    std::ofstream file(filename);
    if (!file.is_open())
    {
        logger.logError("couldn't open %s, sometings went wrong.", filename);
        return;
    }

    for (const auto& section : data) {
        file << "[" << section.first << "]\n";
        for (const auto& pair : section.second) {
            file << pair.first << "=" << pair.second << "\n";
        }
        file << "\n";
    }

    file.close();
}

std::optional<std::string> SimpleIni::getValueBase(const std::string& section, const std::string& key)
{
    auto sectIt = data.find(section);
    if (sectIt != data.end()) {
        auto keyIt = sectIt->second.find(key);
        if (keyIt != sectIt->second.end()) {
            return keyIt->second;
        }
    }
    return std::nullopt;
}

int SimpleIni::getValue(const std::string& section, const std::string& key, const int defaultValue)
{
    auto res = getValueBase(section, key);
    if (res != std::nullopt)
    {
        int value = std::stoi(data[section][key]);
        return value;
    }
    return defaultValue;
}

std::string SimpleIni::getValue(const std::string& section, const std::string& key, const std::string& defaultValue)
{
    auto res = getValueBase(section, key);
    if (res != std::nullopt)
    {
        return data[section][key];
    }
    return defaultValue;
}

void SimpleIni::setValue(const std::string& section, const std::string& key, const int value)
{
    std::string stringValue = std::to_string(value);
    data[section][key] = stringValue;
}

void SimpleIni::setValue(const std::string& section, const std::string& key, const std::string& value)
{
    data[section][key] = value;
}