
#ifndef SKLINECURVE_HPP_
#define SKLINECURVE_HPP_

#include "SkGeometry.hpp"
#include <Drawables/Tangible.hpp>
#include <constraintsSolver/expression.hpp>
#include <geometry/geometry_2d/Line_abstr.hpp>

#include <graphics_utils/VertexArray.hpp>
#include <graphics_utils/VertexBuffer.hpp>
#include <graphics_utils/Shader.hpp>

class SkLineCurve : public Geom2d::Line_abstr, public SkPrimitiveGeometry {
private:
	Geom2d::ExpressionPoint<var_ptr>* mPtA, *mPtB;

	static float kSelDist2;
	static bool kFisrstInst;
	static glm::vec3 kColor, kColorHovered, kColorSelected; // Line color

	glm::vec3 mVertices[2]; // The vertices describing the rendered line

	VertexArray* mVA;
	VertexBuffer* mVB;
	std::shared_ptr<Shader> mShader;
public:
	SkLineCurve(Geom2d::ExpressionPoint<var_ptr>* ptA, Geom2d::ExpressionPoint<var_ptr>* ptB, Geom3d::plane_abstr* pl, bool fixed_);
	~SkLineCurve();

	glm::vec2 posA() { return mPtA->pos(); }
	glm::vec2 posB() { return mPtB->pos(); }
	Geom2d::ExpressionPoint<var_ptr>* ptA() { return mPtA; }
	Geom2d::ExpressionPoint<var_ptr>* ptB() { return mPtB; }

	SelectionPoint closest_2d(glm::vec2 planePos, Camera* cam, glm::vec2 cursorPos, int filter);
	DraggableSelectionPoint closest_2d_draggable(glm::vec2 planePos, Camera* cam, glm::vec2 cursorPos, int filter);
	int selection_rank() { return 3; }
	void move(glm::vec2 start, glm::vec2 end, glm::vec2 pix_mov);
	void set_annotOffset(SkSprite* sp, int ind);
private:
	void init_impl();
	void draw_impl(Camera_ptr cam, int frame, draw_type type = draw_type::ALL);
	void update_impl();
};

#endif