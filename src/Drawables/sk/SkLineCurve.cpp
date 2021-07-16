
#include "SkLineCurve.hpp"

#include <graphics_utils/ShadersPool.hpp>
#include <utils/preferences.hpp>
#include <graphics_utils/GLCall.hpp>

SkLineCurve::SkLineCurve(Geom2d::Point_abstr* ptA, Geom2d::Point_abstr* ptB, geom_3d::plane_abstr_ptr pl, bool fixed_):
	SkPrimitiveGeometry(pl, fixed_),
	mPtA(ptA),
	mPtB(ptB),
	mVA(nullptr),
	mVB(nullptr),
	mShader(nullptr)
{
	mType |= Drawable_types::AXIS;
}

SkLineCurve::~SkLineCurve()
{
	delete mVA;
	delete mVB;
}

void SkLineCurve::init()
{
	mNeed_update = false;
	
	// mLength2 = (pow(mA->x()-mB->x(), 2.0) + pow(mA->y()-mB->y(), 2.0));

	set_name("line");
	mVA = new VertexArray();
	VertexBufferLayout layout;
	layout.add_proprety_float(3);
	mVA->bind();

	mVertices[0] = mBasePlane->to_worldPos(posA());
	mVertices[1] = mBasePlane->to_worldPos(posB());
	mVB = new VertexBuffer(&mVertices[0], sizeof(glm::vec3) * 2);
	mVA->add_buffer(*mVB, layout);

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

void SkLineCurve::draw(Camera_ptr cam, int frame, draw_type type)
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
void SkLineCurve::update()
{
	mNeed_update = false;
	mVertices[0] = mBasePlane->to_worldPos(posA());
	mVertices[1] = mBasePlane->to_worldPos(posB());
	mVB->bind();
	mVB->set(&mVertices[0], sizeof(glm::vec3) * 2);
	mVB->unbind();
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