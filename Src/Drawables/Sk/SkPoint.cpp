
#include "SkPoint.hpp"

#include <Utils/Expunge.hpp>
#include <Graphics_utils/ShadersPool.hpp>
#include <Utils/Preferences.hpp>
#include <Graphics_utils/GLCall.hpp>

float SkPoint::kSelDist2 = 0.0f;
bool SkPoint::kFisrstInst = true;
glm::vec3 SkPoint::kColor = glm::vec3(0.0); 
glm::vec3 SkPoint::kColorHovered = glm::vec3(0.0);
glm::vec3 SkPoint::kColorSelected = glm::vec3(0.0);

SkPoint::SkPoint(glm::vec2 pos, Geom3d::Plane_abstr* pl, bool fixed_)
	: SkPrimitiveGeometry<Geom2d::Point>(pl, fixed_)
{
	mGeom = new Geom2d::Point(pos);
	mType |= Drawable_types::POINT;
	set_name("SkPoint");
}
SkPoint::SkPoint(Geom2d::Point* pt, Geom3d::Plane_abstr* pl, bool fixed_)
	: SkPrimitiveGeometry<Geom2d::Point>(pl, fixed_)
{

}

SkPoint::~SkPoint()
{
	if(mInited) {
		expunge(mVA);
		expunge(mVB);
	}
}

SelPoint SkPoint::closest_2d(glm::vec2 planePos, Camera* cam, glm::vec2 cursorPos, int filter)
{
	if(mType & filter) {
		glm::vec3 worldpos = mBasePlane->to_worldPos(pos());
		if(glm::distance2(cam->world_to_screen(worldpos), cursorPos) < kSelDist2)
			return { this, glm::distance(cam->pos(), worldpos) };
	}
	return SelPoint();
}
SkExpSelPoint SkPoint::closest_2d_draggable(glm::vec2 planePos, Camera* cam, glm::vec2 cursorPos, int filter)
{
	if(mType & filter) {
		glm::vec3 worldpos = mBasePlane->to_worldPos(pos());
		if(glm::distance2(cam->world_to_screen(worldpos), cursorPos) < kSelDist2)
			return SkExpSelPoint(this);//, ExpVec2<Expression_abstr>({x(), y()}));
	}
	return SkExpSelPoint();
}

void SkPoint::set_annotOffset(SkSprite* sp, int ind)
{
	int level = ind / 6;
	float angle = (float)(ind % 6) / 6.0f * M_PI * 2.0 + M_PI_2;
	sp->set_pixelOffset(glm::vec2(std::cos(angle) * (float)(ind) * 25.0f, std::sin(angle) * (float)(ind) * 25.0f));
}

void SkPoint::set(glm::vec2 pt)
{
	mGeom->x().drag(pt.x);
	mGeom->y().drag(pt.y);
	notify_parent(SkNotifiers::SET);
}

std::vector<var_ptr> SkPoint::all_vars()
{
	return {};//{ x(), y() };
}
void SkPoint::release()
{
	mGeom->x().set_dragged(false);
	mGeom->y().set_dragged(false);
}

void SkPoint::init_impl()
{
	mNeed_graphicUpdate = false;
	mVA = new VertexArray();
	VertexBufferLayout layout;
	layout.add_proprety_float(3);
	mVA->bind();

	glm::vec3 pos_tmp = mBasePlane->to_worldPos(pos());
	mVB = new VertexBuffer(&pos_tmp[0], sizeof(glm::vec3));
	mVA->add_buffer(*mVB, layout);
	mVA->unbind();

	mShader = ShadersPool::get_instance().get("point");
	if(!mShader) {
		mShader = Shader::fromFiles_ptr({
		{"Resources/Shaders/Point.vert", GL_VERTEX_SHADER},
		{"Resources/Shaders/SquareExpand.geom", GL_GEOMETRY_SHADER}, 
		{"Resources/Shaders/Point.frag", GL_FRAGMENT_SHADER}}); // Geometry Shader is needed because point is expanded on the gpu
		ShadersPool::get_instance().add("point", mShader);
	}
	if(kFisrstInst) {
		kSelDist2 = Preferences::get_instance().get_float("seldistpoint2");
		Preferences::get_instance().add_callback("seldistpoint2", 
			std::function<void(float)>([this](float val) { kSelDist2 = val; }));
		kColor = Preferences::get_instance().get_vec3("sketchEntityColor");
		Preferences::get_instance().add_callback("sketchEntityColor", 
			std::function<void(glm::vec3)>([this](glm::vec3 val) { kColor = val; }));
		kColorHovered = Preferences::get_instance().get_vec3("sketchEntityColorHovered");
		Preferences::get_instance().add_callback("sketchEntityColorHovered", 
			std::function<void(glm::vec3)>([this](glm::vec3 val) { kColorHovered = val; }));
		kColorSelected = Preferences::get_instance().get_vec3("sketchEntityColorSelected");
		Preferences::get_instance().add_callback("sketchEntityColorSelected", 
			std::function<void(glm::vec3)>([this](glm::vec3 val) { kColorSelected = val; }));

		kFisrstInst = false;
	}
}
void SkPoint::draw_impl(Camera* cam, int frame, draw_type type)
{
	mShader->bind();
	glm::vec4 color = glm::vec4(kColor, 1.0f);
	if(selected()) {
		color = glm::vec4(kColorSelected, 1.0f);
	} else if(hovered()) {
		color = glm::vec4(kColorHovered, 1.0f);
	}
	
	mShader->setUniform4f("u_Color", color);
	mShader->setUniform1f("u_Diameter", 5);
	// mShader->setUniform1f("u_Feather", 0.6);

	if(mShader->lastUsed() != frame) {
		mShader->setUniformMat4f("u_MVP", cam->mvp());
		mShader->setUniform2f("u_Viewport", cam->viewport());
		mShader->set_used(frame);
	}

	mVA->bind();

	GLCall(glDrawArrays(GL_POINTS, 0, 1)); // No indexing needed, a line only has one vertex

	mVA->unbind();
	mShader->unbind();
}
void SkPoint::graphicUpdate_impl()
{
	update_annots();
	mVB->bind();
	glm::vec3 pos_tmp = mBasePlane->to_worldPos(pos());
	mVB->set(&pos_tmp[0], sizeof(glm::vec3));
	mVB->unbind();
}

void SkPoint::move_impl(glm::vec2 delta)
{
	set(pos() + delta);
}
