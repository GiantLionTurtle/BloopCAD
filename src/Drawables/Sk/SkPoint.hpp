
#ifndef SKPOINT_HPP_
#define SKPOINT_HPP_

#include "SkGeometry.hpp"
#include <Drawables/Tangible.hpp>
#include <constraintsSolver/expression.hpp>
#include <geometry/geometry_2d/Point_abstr.hpp>

#include <graphics_utils/VertexArray.hpp>
#include <graphics_utils/VertexBuffer.hpp>
#include <graphics_utils/Shader.hpp>

class SkPoint : public Geom2d::ExpressionPoint<var_ptr>, public SkPrimitiveGeometry {
private:
	static float kSelDist2;
	static bool kFisrstInst;
	static glm::vec3 kColor, kColorHovered, kColorSelected; // Point color

	VertexArray* mVA;
	VertexBuffer* mVB;
	std::shared_ptr<Shader> mShader;
public:
	SkPoint(glm::vec2 pos, Geom3d::plane_abstr* pl, bool fixed_);
	~SkPoint();

	SelectionPoint closest_2d(glm::vec2 planePos, Camera* cam, glm::vec2 cursorPos, int filter);
	DraggableSelectionPoint closest_2d_draggable(glm::vec2 planePos, Camera* cam, glm::vec2 cursorPos, int filter);
	int selection_rank() { return 5; }
	virtual void set_annotPos(SkSprite* sp) { sp->set(pos()); }
	void set_annotOffset(SkSprite* sp, int ind);

	void set(glm::vec2 pt);

	std::vector<var_ptr> all_vars();
protected:
	void init_impl();
	void draw_impl(Camera_ptr cam, int frame, draw_type type = draw_type::ALL);
	void update_impl();

	void move_impl(glm::vec2 start, glm::vec2 end, glm::vec2 pix_mov);
};

#endif