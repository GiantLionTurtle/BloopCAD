
#ifndef TANGIBLE_HPP_
#define TANGIBLE_HPP_

#include "Drawable.hpp"

#include <Graphics_utils/Shader.hpp>
#include <Graphics_utils/VertexArray.hpp>
#include <Graphics_utils/VertexBuffer.hpp>
#include <Graphics_utils/IndexBuffer.hpp>

class Tangible : virtual public Drawable {
public:
	Tangible();

	// void draw(Camera* cam, int frame, draw_type type = draw_type::ALL);
};

#endif