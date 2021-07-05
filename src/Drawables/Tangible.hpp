
#ifndef TANGIBLE_HPP_
#define TANGIBLE_HPP_

#include "Drawable.hpp"

#include <graphics_utils/Shader.hpp>
#include <graphics_utils/VertexArray.hpp>
#include <graphics_utils/VertexBuffer.hpp>
#include <graphics_utils/IndexBuffer.hpp>

class Tangible : public Drawable {
protected:
	bool mGPU_outOfSync; // If the geometry on the GPU is not what it should be (e.g. a point was moved or anything really)
public:
	Tangible();

	void update();

	virtual void sync_GPU() = 0;
};

#endif