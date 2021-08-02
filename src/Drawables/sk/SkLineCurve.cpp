
#include "SkLineCurve.hpp"

#include <graphics_utils/ShadersPool.hpp>
#include <utils/preferences.hpp>
#include <graphics_utils/GLCall.hpp>

float SkLineCurve::kSelDist2 = 0.0f;
bool SkLineCurve::kFisrstInst = true;
glm::vec3 SkLineCurve::kColor = glm::vec3(0.0); 
glm::vec3 SkLineCurve::kColorHovered = glm::vec3(0.0);
glm::vec3 SkLineCurve::kColorSelected = glm::vec3(0.0);

SkLineCurve::SkLineCurve(Geom2d::ExpressionPoint<var_ptr>* ptA, Geom2d::ExpressionPoint<var_ptr>* ptB, Geom3d::plane_abstr* pl, bool fixed_):
	SkPrimitiveGeometry(pl, fixed_),
	mPtA(ptA),
	mPtB(ptB),
	mVA(nullptr),
	mVB(nullptr),
	mShader(nullptr)
{
	mType |= Drawable_types::AXIS | Drawable_types::CURVE;
	set_name("SkLineCurve");
}

SkLineCurve::~SkLineCurve()
{
	delete mVA;
	delete mVB;
}

SelectionPoint SkLineCurve::closest_2d(glm::vec2 planePos, Camera* cam, glm::vec2 cursorPos, int filter)
{
	if(mType & filter) {
		glm::vec3 worldpos = mBasePlane->to_worldPos(closest_to_point(planePos));
		if(glm::distance2(cam->world_to_screen(worldpos), cursorPos) < kSelDist2)
			return { this, glm::distance(cam->pos(), worldpos) };
	}
	return SelectionPoint();
}
DraggableSelectionPoint SkLineCurve::closest_2d_draggable(glm::vec2 planePos, Camera* cam, glm::vec2 cursorPos, int filter)
{
	if(mType & filter) {
		glm::vec3 worldpos = mBasePlane->to_worldPos(closest_to_point(planePos));
		if(glm::distance2(cam->world_to_screen(worldpos), cursorPos) < kSelDist2)
			return { this };
	}
	return DraggableSelectionPoint();
}
void SkLineCurve::move(glm::vec2 start, glm::vec2 end, glm::vec2 pix_mov)
{
	mPtA->set(mPtA->pos() + end - start);
	mPtB->set(mPtB->pos() + end - start);
}

void SkLineCurve::set_annotOffset(SkSprite* sp, int ind)
{
	glm::vec2 dir = glm::normalize(mPtA->pos() - mPtB->pos());
	glm::vec2 normal = glm::cross(glm::vec3(dir, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	int line_side = ind % 2 == 0 ? 1 : -1;
	bool icon_dir = line_side == -1 ? ind % 4 ? -1 : 1 : ind % 3 ? -1 : 1;
	float dir_offset = line_side == 1 ? ind : ind - 1;
	sp->set_pixelOffset(dir * 25.0f * (float)icon_dir * dir_offset + normal * 25.0f * (float)line_side);
}

void SkLineCurve::init_impl()
{
	mNeed_update = false;
	
	// mLength2 = (pow(mA->x()-mB->x(), 2.0) + pow(mA->y()-mB->y(), 2.0));

	mVA = new VertexArray();
	VertexBufferLayout layout;
	layout.add_proprety_float(3);
	mVA->bind();

	mVertices[0] = mBasePlane->to_worldPos(posA());
	mVertices[1] = mBasePlane->to_worldPos(posB());
	mVB = new VertexBuffer(&mVertices[0], sizeof(glm::vec3) * 2);
	mVA->add_buffer(*mVB, layout);
	mVA->unbind();

	mShader = ShadersPool::get_instance().get("line");
	if(!mShader) {
		mShader = Shader::fromFiles_ptr({
		{"resources/shaders/lineShader.vert", GL_VERTEX_SHADER},
		{"resources/shaders/lineShader.geom", GL_GEOMETRY_SHADER}, 
		{"resources/shaders/lineShader.frag", GL_FRAGMENT_SHADER}}); // Geometry Shader is needed because line is expanded on the gpu
		ShadersPool::get_instance().add("line", mShader);
	}
	if(kFisrstInst) {
		kSelDist2 = preferences::get_instance().get_float("seldistcurve2");
		preferences::get_instance().add_callback("seldistcurve2", std::function<void(float)>([this](float val) { kSelDist2 = val; }));
		kColor = preferences::get_instance().get_vec3("sketchEntityColor");
		preferences::get_instance().add_callback("sketchEntityColor", std::function<void(glm::vec3)>([this](glm::vec3 val) { kColor = val; }));
		kColorHovered = preferences::get_instance().get_vec3("sketchEntityColorHovered");
		preferences::get_instance().add_callback("sketchEntityColorHovered", std::function<void(glm::vec3)>([this](glm::vec3 val) { kColorHovered = val; }));
		kColorSelected = preferences::get_instance().get_vec3("sketchEntityColorSelected");
		preferences::get_instance().add_callback("sketchEntityColorSelected", std::function<void(glm::vec3)>([this](glm::vec3 val) { kColorSelected = val; }));

		kFisrstInst = false;
	}
}

void SkLineCurve::draw_impl(Camera_ptr cam, int frame, draw_type type)
{
	mShader->bind();
	glm::vec4 color = glm::vec4(kColor, 1.0f);
	if(selected()) {
		color = glm::vec4(kColorSelected, 1.0f);
	} else if(hovered()) {
		color = glm::vec4(kColorHovered, 1.0f);
	}

	mShader->setUniform4f("u_Color", color);
	mShader->setUniform1f("u_LineWidth", 5); 	// Line width is in pixel
	mShader->setUniform1f("u_Feather", 0.6);	// Amount of feather, the formula might change so for the time being, higher is more feather

	if(mShader->lastUsed() != frame) {
		mShader->setUniformMat4f("u_MVP", cam->mvp());
		mShader->setUniform2f("u_Viewport", cam->viewport());
		mShader->set_used(frame);
	}

	mVA->bind();

	GLCall(glDrawArrays(GL_LINES, 0, 2)); // No indexing needed, a line only has two vertices

	mVA->unbind();
	mShader->unbind();
}
void SkLineCurve::update_impl()
{
	update_annots();
	mNeed_update = false;
	mVertices[0] = mBasePlane->to_worldPos(posA());
	mVertices[1] = mBasePlane->to_worldPos(posB());
	mVB->bind();
	mVB->set(&mVertices[0], sizeof(glm::vec3) * 2);
	mVB->unbind();
}
