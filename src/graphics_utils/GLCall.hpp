
#ifndef GLCALL_HPP_
#define GLCALL_HPP_

#include "../errorLogger.hpp"

#include <epoxy/gl.h>

void GLClearErrors();
bool GLLogCall(std::string const& call, std::string const& file, std::string const& function, unsigned int line);

#define GLASSERT_NO_SHOW(x) if(!(x)) std::cin.get();
#define GLCall(x) GLClearErrors();\
  x;\
  GLASSERT_NO_SHOW(GLLogCall(#x, __FILE__, __FUNCTION__, __LINE__))

#endif