#ifndef CommonLibs_Ini_H
#define CommonLibs_Ini_H

#include <string>
#include <map>
#include <optional>

class SimpleIni
{
public:
	SimpleIni(const std::string& fileName);
	bool read();
	void write();

	template<typename T>
	T getSingleValue(const std::string& section, const std::string& key, T defaultValue)
	{
		return getValue(section, key, defaultValue);
	}

	template<typename T>
	void setSingleValue(const std::string& section, const std::string& key, T value)
	{
		setValue(section, key, value);
	}
private:
	std::string filename;
	std::map<std::string, std::map<std::string, std::string>> data;

	std::optional<std::string> getValueBase(const std::string& section, const std::string& key);
	int getValue(const std::string& section, const std::string& key, const int defaultValue);
	std::string getValue(const std::string& section, const std::string& key, const std::string& defaultValue);
	void setValue(const std::string& section, const std::string& key, const int value);
	void setValue(const std::string& section, const std::string& key, const std::string& value);
};

extern SimpleIni ini;

#endif // !CommonLibs_Ini_H