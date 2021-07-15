
#ifndef SKPOINT_HPP_
#define SKPOINT_HPP_

#include "SkGeometry.hpp"
#include <Drawables/Tangible.hpp>
#include <constraintsSolver/expression.hpp>
#include <geometry/geometry_2d/point_abstr.hpp>

#include <graphics_utils/VertexArray.hpp>
#include <graphics_utils/VertexBuffer.hpp>
#include <graphics_utils/Shader.hpp>

class ExpressionPoint : public geom_2d::point_abstr {
protected:
	expression_ptr mX, mY;
public:
	ExpressionPoint(expression_ptr x_, expression_ptr y_):
		mX(x_),
		mY(y_)
	{}

	glm::vec2 pos() { return glm::vec2(mX->eval(), mY->eval()); }

	expression_ptr x() { return mX; }
	expression_ptr y() { return mY; }
};

class SkPoint : public ExpressionPoint, public SkGeometry {
private:
	static float kSelDist2;
	static bool kFisrstInst;
	static glm::vec3 kColor, kColorHovered, kColorSelected; // Point color

	VertexArray* mVA;
	VertexBuffer* mVB;
	std::shared_ptr<Shader> mShader;
public:
	SkPoint(glm::vec2 pos, geom_3d::plane_abstr_ptr pl, bool fixed_);
	~SkPoint();

	void init();

	void draw(Camera_ptr cam, int frame, draw_type type = draw_type::ALL);

	void update();

	SelectionPoint closest_2d(glm::vec2 planePos, Camera* cam, glm::vec2 cursorPos, int filter);
	DraggableSelectionPoint closest_2d_draggable(glm::vec2 planePos, Camera* cam, glm::vec2 cursorPos, int filter);
};

#endif