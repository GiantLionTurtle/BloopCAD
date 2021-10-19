#ifndef DEBUGUTILS_HPP_
#define DEBUGUTILS_HPP_

#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>

#define VERBOSE_NONE 		0
#define VERBOSE_WARNINGS 	1
#define VERBOSE_CONFIGS 	2
#define VERBOSE_STEPS 		3
#define VERBOSE_INNERSTEPS 	4
#define VERBOSE_ALL 		5

// #define DISABLE_VERBOSE
#ifndef VERBOSE_LEVEL
#define VERBOSE_LEVEL VERBOSE_NONE
#endif

#ifndef DISABLE_VERBOSE
#define VERBOSE_INTERNAL(level, msg, comp) \
	if(level <= comp) \
		std::cout<<msg<<std::endl;
#else
#define VERBOSE_INTERNAL(level, msg, comp)
#endif

#ifdef VERBOSE_LEVEL
#define verbose(level, msg) VERBOSE_INTERNAL(level, msg, VERBOSE_LEVEL)
#else
#define verbose(level, msg) 
#endif

/*
	@class ErrorLogger logs messages in stream and or console streams

	@note : This class is a singleton
*/
class ErrorLogger {
private:
	int mMode; // How to log things
	std::ofstream mFileStream; // File stream if needed
public:
	enum modes { CERR = 1 << 0, FILE = 1 << 1 }; // All the possible modes	

	ErrorLogger(const ErrorLogger&) = delete;

	/*
		@function get_instance gives the singleton instance

		@return : The singleton instance
	*/
	static ErrorLogger& get_instance()
	{
		static ErrorLogger logger;
		return logger;
	}

	/*
		@function init inits the logger with console stream only
	*/
	void init()
	{
		mMode = modes::CERR;
	}
	/*
		@function init inits the logger with a file stream and optionaly a console stream

		@param filepath : 	The path to the file into which to stream
		@param print : 		Whether or not to stream in the console
	*/
	void init(std::string const& filepath, bool print)		
	{
		mMode = print ? (modes::CERR | modes::FILE) : modes::FILE;
		mFileStream = std::ofstream(filepath);
	}

	/*
		@function log logs a message 
		
		@param what : 		The log type
		@param file : 		The file where the log was called
		@param function : 	The function that logged
		@param line : 		The line at which the log was called
		@param msg : 		The log message
	*/
	void log(std::string const& what, std::string const& file, std::string const& function, unsigned int line, std::string const& msg)
	{
		std::string log = what + ": in file \"" + file + "\" in function \"" + function + "\" at line " + std::to_string(line) + ": " + msg;
		if (mMode & modes::CERR)
			cerr_log(log);
		if (mMode & modes::FILE)
			ofstream_log(log);
	}
private:
	/*
		@function ErrorLogger creates the error logger
	*/
	ErrorLogger()
	{}

	/*
		@function cerr_log logs a string to the console
	*/
	void cerr_log(std::string const& log)
	{
		std::cerr << log << std::endl;
	}
	/*
		@function ofstream_log logs a string to the file stream
	*/
	void ofstream_log(std::string const& log)
	{
		mFileStream << log << std::endl;
	}
};

// Macro chooser inspired by https://stackoverflow.com/questions/3046889/optional-parameters-with-c-macros
#define LOG_ERROR_EMPTY()				ErrorLogger::get_instance().log("ERROR", __FILE__, __FUNCTION__, __LINE__, "Unknown error."); exit(EXIT_FAILURE)
#define LOG_ERROR_EXIT_FAILURE(msg) 	ErrorLogger::get_instance().log("ERROR", __FILE__, __FUNCTION__, __LINE__, msg); exit(EXIT_FAILURE)
#define LOG_ERROR_CODE(msg, exitCode)	ErrorLogger::get_instance().log("ERROR", __FILE__, __FUNCTION__, __LINE__, msg); exit(exitCode)
#define LOG_ERROR_CHOOSER(nothing, msg, exitCode, option, ...) option

#define LOG_ERROR(...)				LOG_ERROR_CHOOSER(, ##__VA_ARGS__, LOG_ERROR_CODE(__VA_ARGS__), LOG_ERROR_EXIT_FAILURE(__VA_ARGS__), LOG_ERROR_EMPTY(__VA_ARGS__));
#define LOG_WARNING(msg) 			if(VERBOSE_LEVEL >= VERBOSE_WARNINGS) ErrorLogger::get_instance().log("WARNING", __FILE__, __FUNCTION__, __LINE__, msg)
#define BLOOP_MARKER				ErrorLogger::get_instance().log("MARKER", __FILE__, __FUNCTION__, __LINE__, "")

// #define RELEASE_MODE
#ifdef RELEASE_MODE
#define DEBUG_ASSERT(condition, reaction)
#else
//#define DEBUG_ASSERT(condition, reaction) std::cout<<"a.\n"; if(static_cast<bool>(condition)) reaction; std::cout<<"b\n";
#define DEBUG_ASSERT(x, msg) { if(!static_cast<bool>(x)) { ErrorLogger::get_instance().log("Assertion failed: ", __FILE__, __FUNCTION__, __LINE__, msg); exit(EXIT_FAILURE); } }
#endif // !RELEASE_MODE

#endif // !ERRORLOGGER_HPP_