
#ifndef TANGIBLE_HPP_
#define TANGIBLE_HPP_

#include "Drawable.hpp"

#include <graphics_utils/Shader.hpp>
#include <graphics_utils/VertexArray.hpp>
#include <graphics_utils/VertexBuffer.hpp>
#include <graphics_utils/IndexBuffer.hpp>

class Tangible : virtual public Drawable {
public:
	Tangible();

	// void draw(Camera_ptr cam, int frame, draw_type type = draw_type::ALL);
};

#endif