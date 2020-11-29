#ifndef ERRORLOGGER_HPP_
#define ERRORLOGGER_HPP_

#include <string>
#include <iostream>
#include <fstream>

//TODO: maybe redo whith inheritance of a logger class isntead of a switch
class errorLogger {
private:
	int mMode;
	std::ofstream mFileStream;
public:
	enum modes { CERR = 1 << 0, FILE = 1 << 1 };

	errorLogger(const errorLogger&) = delete;

	static errorLogger& get_instance()
	{
		static errorLogger logger;
		return logger;
	}

	void init()
	{
		mMode = modes::CERR;
	}
	void init(std::string filepath, bool print)		
	{
		mMode = print ? (modes::CERR | modes::FILE) : modes::FILE;
		mFileStream = std::ofstream(filepath);
	}

	void log(std::string const& what, std::string const& file, std::string const& function, unsigned int line, std::string const& msg)
	{
		std::string log = what + ": in file \"" + file + "\" in function \"" + function + "\" at line " + std::to_string(line) + ": " + msg;
		if (mMode & modes::CERR)
			cerr_log(log);
		if (mMode & modes::FILE)
			ofstream_log(log);
	}
private:
	errorLogger()
	{}

	void cerr_log(std::string log)
	{
		std::cerr << log << std::endl;
	}
	void ofstream_log(std::string log)
	{
		mFileStream << log << std::endl;
	}
};

// Macro chooser inspired by https://stackoverflow.com/questions/3046889/optional-parameters-with-c-macros
#define LOG_ERROR_EMPTY()				errorLogger::get_instance().log("ERROR", __FUNCTION__, __FILE__, __LINE__, "Unknown error."); exit(EXIT_FAILURE)
#define LOG_ERROR_EXIT_FAILURE(msg) 	errorLogger::get_instance().log("ERROR", __FUNCTION__, __FILE__, __LINE__, msg); exit(EXIT_FAILURE)
#define LOG_ERROR_CODE(msg, exitCode)	errorLogger::get_instance().log("ERROR", __FUNCTION__, __FILE__, __LINE__, msg); exit(exitCode)
#define LOG_ERROR_CHOOSER(nothing, msg, exitCode, option, ...) option

#define LOG_ERROR(...)				LOG_ERROR_CHOOSER(, ##__VA_ARGS__, LOG_ERROR_CODE(__VA_ARGS__), LOG_ERROR_EXIT_FAILURE(__VA_ARGS__), LOG_ERROR_EMPTY(__VA_ARGS__));
#define LOG_WARNING(msg) 			errorLogger::get_instance().log("WARNING", __FUNCTION__, __FILE__, __LINE__, msg)
#define BLOOP_MARKER				errorLogger::get_instance().log("MARKER", __FUNCTION__, __FILE__, __LINE__, "")

#endif