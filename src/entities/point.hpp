
#ifndef POINT_HPP_
#define POINT_HPP_

#include "entity.hpp"
#include "point_abstract.hpp"

#include <glm/glm.hpp>

class point : public entity, public point_abstract {
private:
	const glm::vec3 mColor = glm::vec3(0.0f, 0.89f, 0.725f); // Point color

	std::shared_ptr<vertexBuffer> 	mVB; // The vertex buffer to contain the vertices on the gpu
	bool mRequire_VBUpdate;
	std::shared_ptr<vertexArray> 	mVA; // The vertex array to contain the attributes on the gpu
	std::shared_ptr<shader> 		mShader, mSelectionShader; // Both shaders to draw the point and it's selection 
public:
	point(point_abstract const& basePoint);
	~point() {}

	virtual void set_pos(glm::vec3 pos);
	virtual void set_pos(point_abstract const& other);

	void update_VB();
protected:
	void draw_impl(std::shared_ptr<camera> cam, int frame);
	void draw_selection_impl(std::shared_ptr<camera> cam, int frame);
};

#endif