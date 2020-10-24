
#include "GLCall.hpp"

void GLClearErrors()
{
  while(glGetError() != GL_NO_ERROR);
}

bool GLLogCall(std::string const& call, std::string const& file, std::string const& function, unsigned int line)
{
	if(GLenum error = glGetError()) {
		errorLogger::get_instance().log("OpenGL error", file, function, line, "while calling " + call + ": " + std::to_string(error));
		return false;
	}
	return true;
}