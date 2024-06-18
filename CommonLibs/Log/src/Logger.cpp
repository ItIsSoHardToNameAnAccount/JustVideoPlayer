#include "Logger.h"

const std::string logFile = "log.txt";

#ifdef _DEBUG
Logger logger(LogLevel::DEBUG, logFile);
#else
Logger logger(LogLevel::RELEASE, logFile);
#endif // _DEBUG

Logger::Logger(LogLevel level, const std::string& outputFilePath)
{
	logLevel = level;
	outputFile = std::ofstream(outputFilePath);
}

void Logger::setLevel(LogLevel level)
{
	logLevel = level;
}

void Logger::ProcessArguments(std::stringstream& ss)
{
	// Base case for the recursive function to stop the recursion
}