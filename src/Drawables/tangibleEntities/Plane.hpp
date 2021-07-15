
#ifndef PLANE_HPP_
#define PLANE_HPP_

#include <Drawables/Drawable.hpp>
#include <geometry/geometry_3d/plane_abstr.hpp>

#include <graphics_utils/VertexArray.hpp>
#include <graphics_utils/VertexBuffer.hpp>
#include <graphics_utils/Shader.hpp>

#include <glm/glm.hpp>

#include <memory>
#include <string>
#include <iostream>

/*
	@class plane describes a plane drawn on screen
	@parent : entity
	@parent : plane_abstract
*/
class Plane : public Drawable, public geom_3d::plane_abstr {
private:
	const glm::vec3 mColorA = glm::vec3(1.0f, 0.35f, 0.90f), 	// Both colors of a plane, depending on the side from which it is observed
					mColorB = glm::vec3(0.35f, 1.0f, 0.78f); 					// Maybe it should be an option

	glm::vec3 mVertices[4];	// The four vertices of the rectangle that represents the plane
	unsigned int mIndices[6];

	VertexArray* mVA;
	VertexBuffer* mVB;
	IndexBuffer* mIB;
	std::shared_ptr<Shader> mShader;
public:
	/*
		@function plane creates a plane 

		@param plane_ : 				The underlaying plane abstraction
		@param label [defaults to ""] : The label of the plane (not supported yet)
	*/
	Plane(plane_abstr const& plane_);
	virtual ~Plane() {}

	void init();
	void draw(Camera_ptr cam, int frame, draw_type type = draw_type::ALL);
	void update();

	virtual int selection_rank() { return 10; }
	SelectionPoint closest(glm::vec2 cursor, Camera* cam, glm::vec3 cam_ray, int filter);
protected:
	void init_buffers();
};

#endif