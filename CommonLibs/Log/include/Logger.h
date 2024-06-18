#ifndef CommonLibs_Logger_H
#define CommonLibs_Logger_H

#include <string>
#include <iostream>
#include <fstream>
#include <mutex>
#include <sstream>
#include <iomanip>

enum class LogLevel
{
	DEBUG,
	RELEASE,
	INFO,
	WARNING,
	ERROR
};

class Logger
{
public:
	Logger(LogLevel level, const std::string& outputFilePath);
	void setLevel(LogLevel level);

	template<typename... Args>
	void logDebug(const std::string& format, Args... args)
	{
		LogLevel level = LogLevel::DEBUG;
		if (level < logLevel)
		{
			return;
		}

		log(level, format, args...);
	}

	template<typename... Args>
	void logInfo(const std::string& format, Args... args)
	{
		LogLevel level = LogLevel::INFO;
		if (level < logLevel)
		{
			return;
		}

		log(level, format, args...);
	}

	template<typename... Args>
	void logWarning(const std::string& format, Args... args)
	{
		LogLevel level = LogLevel::WARNING;
		if (level < logLevel)
		{
			return;
		}

		log(level, format, args...);
	}

	template<typename... Args>
	void logError(const std::string& format, Args... args)
	{
		LogLevel level = LogLevel::ERROR;
		if (level < logLevel)
		{
			return;
		}

		log(level, format, args...);
	}
private:
	LogLevel logLevel;
	std::ofstream outputFile;
	std::mutex logMutex;

	template<typename T>
	void ProcessArgument(std::stringstream& ss, const T& value) 
	{
		ss << value;
	}

	template<typename T, typename... Args>
	void ProcessArguments(std::stringstream& ss, const T& first, Args... args) 
	{
		ProcessArgument(ss, first);
		ProcessArguments(ss, args...);
	}

	void ProcessArguments(std::stringstream& ss);

	template<typename... Args>
	std::string FormatString(const std::string& format, Args... args)
	{
		std::stringstream formattedString;
		std::stringstream formatStream(format);
		std::string token;
		int argCount = 0;

		// Iterate over the format string and replace placeholders with arguments
		while (std::getline(formatStream, token, '%')) {
			if (!token.empty()) {
				formattedString << token;
			}
			if (formatStream.peek() != '%') {
				switch (argCount) {
				case 0: ProcessArguments(formattedString, args...); break;
					// Add more cases if you have more placeholders
					// Note: This is a basic implementation and might need adjustments based on specific needs
				}
				argCount++;
			}
			else {
				formatStream.ignore(); // Skip the escaped '%'
			}
		}

		return formattedString.str();
	}

	template<typename... Args>
	void log(LogLevel level, const std::string& format, Args... args)
	{
		std::string message = FormatString(format, args...);
		auto now = std::chrono::system_clock::now();
		auto now_c = std::chrono::system_clock::to_time_t(now);
		std::stringstream ss;
		ss << std::put_time(std::localtime(&now_c), "%Y-%m-%d %X");

		std::lock_guard<std::mutex> guard(logMutex);
		switch (level)
		{
		case LogLevel::DEBUG:
			outputFile << "[DEBUG] [" << ss.str() << "]" << message << std::endl;
			break;
		case LogLevel::RELEASE:
			break;
		case LogLevel::INFO:
			outputFile << "[INFO] [" << ss.str() << "]" << message << std::endl;
			break;
		case LogLevel::WARNING:
			outputFile << "[WARNING] [" << ss.str() << "]" << message << std::endl;
			break;
		case LogLevel::ERROR:
			outputFile << "[ERROR] [" << ss.str() << "]" << message << std::endl;
			break;
		default:
			break;
		}
		outputFile.flush();
	}
};

extern Logger logger;

#endif // !CommonLibs_Logger_H