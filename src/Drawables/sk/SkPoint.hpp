
#ifndef SKPOINT_HPP_
#define SKPOINT_HPP_

#include "SkGeometry.hpp"
#include <Drawables/Tangible.hpp>
#include <constraintsSolver/expression.hpp>
#include <geometry/geometry_2d/Point_abstr.hpp>

#include <graphics_utils/VertexArray.hpp>
#include <graphics_utils/VertexBuffer.hpp>
#include <graphics_utils/Shader.hpp>

template<typename eT>
class ExpressionPoint : public Geom2d::Point_abstr {
protected:
	eT mX, mY;
public:
	ExpressionPoint(eT x_, eT y_):
		mX(x_),
		mY(y_)
	{}

	glm::vec2 pos() { return glm::vec2(mX->eval(), mY->eval()); }

	eT x() { return mX; }
	eT y() { return mY; }
};

class SkPoint : public ExpressionPoint<var_ptr>, public SkPrimitiveGeometry {
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
	void move(glm::vec2 start, glm::vec2 end, glm::vec2 pix_mov);

	void set(glm::vec2 pt);
};

#endif