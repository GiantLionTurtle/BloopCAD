
#ifndef GLCALL_HPP_
#define GLCALL_HPP_

#include <utils/errorLogger.hpp>

#include <epoxy/gl.h>

/*
	@function GLClearErrors makes sure openGL has no error accumulated, since it can only give the newest one
*/
void GLClearErrors();
/*
	@function GLLogCall logs a warning if an internal error is detected in openGL
*/
bool GLLogCall(std::string const& call, std::string const& file, std::string const& function, unsigned int line);

/*
	@macro GLCall calls an openGL function and logs a warning if a new error appears
*/
#define GLCall(x) GLClearErrors();\
	x;\
	GLLogCall(#x, __FILE__, __FUNCTION__, __LINE__)

// #define LOG_EVERY_GLCALL // Uncomment to have every openGL call logged, error or no error

#endif