
#include "SkCircle.hpp"

#include <Graphics_utils/ShadersPool.hpp>
#include <Utils/Preferences.hpp>
#include <Graphics_utils/GLCall.hpp>
#include <Utils/Debug_util.hpp>

struct circleData {
	glm::vec3 pos;
	float radius;
	glm::vec3 v, w;
};

bool SkCircle::kFisrstInst = true;
glm::vec3 SkCircle::kColor = glm::vec3(0.0); 
glm::vec3 SkCircle::kColorHovered = glm::vec3(0.0);
glm::vec3 SkCircle::kColorSelected = glm::vec3(0.0);

SkCircle::SkCircle(glm::vec2 center_, float radius_, Geom3d::Plane_abstr* pl, bool fixed_)
	: SkCurve<LinearFixed_indexer<SkPoint*, 1>, Geom2d::Circle>(pl, fixed_, new Geom2d::Circle(center_, radius_))
{
	handle(0) = new SkPoint(mGeom->center(), pl, fixed_);
	set_name("SkCircle");
}

void SkCircle::set_annotOffset(SkSprite* sp, int ind)
{
	// probably change that so that the center point's constraints and the circle's constraints are clearly separated
	int level = ind / 6;
	float angle = (float)(ind % 6) / 6.0f * M_PI * 2.0 + M_PI_2;
	sp->set_pixelOffset(glm::vec2(std::cos(angle) * (float)(ind) * 25.0f, std::sin(angle) * (float)(ind) * 25.0f));
}

void SkCircle::init_impl()
{
	mNeed_graphicUpdate = false;
	
	mVA = new VertexArray();
	VertexBufferLayout layout;
	layout.add_proprety_float(3);
	layout.add_proprety_float(1);
	layout.add_proprety_float(3);
	layout.add_proprety_float(3);
	mVA->bind();
	circleData tosend = { basePlane()->to_worldPos(center_pos()), radius_val(), basePlane()->v(), basePlane()->w() };
	mVB = new VertexBuffer(&tosend, sizeof(circleData));
	mVA->add_buffer(*mVB, layout);
	mVA->unbind();

	mShader = ShadersPool::get_instance().get("circle");
	if(!mShader) {
		mShader = Shader::fromFiles_ptr({
		{"Resources/Shaders/Circle.vert", GL_VERTEX_SHADER},
		{"Resources/Shaders/Circle.tesc", GL_TESS_CONTROL_SHADER},
		{"Resources/Shaders/Circle.tese", GL_TESS_EVALUATION_SHADER},
		{"Resources/Shaders/Line.geom", GL_GEOMETRY_SHADER}, 
		{"Resources/Shaders/Line.frag", GL_FRAGMENT_SHADER}}); // Geometry Shader is needed because line is expanded on the gpu
		ShadersPool::get_instance().add("circle", mShader);
	}
	if(kFisrstInst) {
		kSelDist2 = Preferences::get_instance().get_float("seldistcurve2");
		Preferences::get_instance().add_callback("seldistcurve2", std::function<void(float)>([this](float val) { kSelDist2 = val; }));
		kColor = Preferences::get_instance().get_vec3("sketchEntityColor");
		Preferences::get_instance().add_callback("sketchEntityColor", std::function<void(glm::vec3)>([this](glm::vec3 val) { kColor = val; }));
		kColorHovered = Preferences::get_instance().get_vec3("sketchEntityColorHovered");
		Preferences::get_instance().add_callback("sketchEntityColorHovered", std::function<void(glm::vec3)>([this](glm::vec3 val) { kColorHovered = val; }));
		kColorSelected = Preferences::get_instance().get_vec3("sketchEntityColorSelected");
		Preferences::get_instance().add_callback("sketchEntityColorSelected", std::function<void(glm::vec3)>([this](glm::vec3 val) { kColorSelected = val; }));

		kFisrstInst = false;
	}
}

void SkCircle::draw_impl(Camera* cam, int frame, draw_type type)
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
		mShader->setUniform1f("u_Scale", cam->fscale());
		mShader->set_used(frame);
	}

	mVA->bind();

	GLCall(glPatchParameteri(GL_PATCH_VERTICES, 1));
	GLCall(glDrawArrays(GL_PATCHES, 0, 1));

	mVA->unbind();
	mShader->unbind();
}
void SkCircle::graphicUpdate_impl()
{
	update_annots();
	mNeed_graphicUpdate = false;
	circleData tosend = { basePlane()->to_worldPos(center_pos()), radius_val(), basePlane()->v(), basePlane()->w() };
	mVB->bind();
	mVB->set(&tosend, sizeof(circleData));
	mVB->unbind();
}
