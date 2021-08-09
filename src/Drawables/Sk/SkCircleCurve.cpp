
#include "SkCircleCurve.hpp"

#include <graphics_utils/ShadersPool.hpp>
#include <utils/preferences.hpp>
#include <graphics_utils/GLCall.hpp>

template<>
float SkCurve<std::array<Geom2d::ExpressionPoint<var_ptr>*, 1>, SkCircleCurve>::kSelDist2 = 0.0f;
bool SkCircleCurve::kFisrstInst = true;
glm::vec3 SkCircleCurve::kColor = glm::vec3(0.0); 
glm::vec3 SkCircleCurve::kColorHovered = glm::vec3(0.0);
glm::vec3 SkCircleCurve::kColorSelected = glm::vec3(0.0);

struct circleData {
	glm::vec3 pos;
	float radius;
	glm::vec3 v, w;
};

SkCircleCurve::SkCircleCurve(Geom3d::plane_abstr* pl, bool fixed_):
    SkCurve<std::array<Geom2d::ExpressionPoint<var_ptr>*, 1>, SkCircleCurve>(pl, fixed_),
	mRadius(std::make_shared<expression_var>(0.0f))
{
	set_name("SkCircleCurve");
}

SkCircleCurve::~SkCircleCurve()
{

}

void SkCircleCurve::set_annotOffset(SkSprite* sp, int ind)
{
    // probably change that so that the center point's constraints and the circle's constraints are clearly separated
	int level = ind / 6;
	float angle = (float)(ind % 6) / 6.0f * M_PI * 2.0 + M_PI_2;
	sp->set_pixelOffset(glm::vec2(std::cos(angle) * (float)(ind) * 25.0f, std::sin(angle) * (float)(ind) * 25.0f));
}

void SkCircleCurve::init_impl()
{
	mNeed_update = false;
	
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
		{"resources/shaders/CircleShader.vert", GL_VERTEX_SHADER},
		{"resources/shaders/CircleShader.tesc", GL_TESS_CONTROL_SHADER},
		{"resources/shaders/CircleShader.tese", GL_TESS_EVALUATION_SHADER},
		{"resources/shaders/lineShader.geom", GL_GEOMETRY_SHADER}, 
		{"resources/shaders/lineShader.frag", GL_FRAGMENT_SHADER}}); // Geometry Shader is needed because line is expanded on the gpu
		ShadersPool::get_instance().add("circle", mShader);
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

void SkCircleCurve::draw_impl(Camera_ptr cam, int frame, draw_type type)
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
	mShader->setUniform1i("u_Outer1", 100);

	if(mShader->lastUsed() != frame) {
		mShader->setUniformMat4f("u_MVP", cam->mvp());
		mShader->setUniform2f("u_Viewport", cam->viewport());
		mShader->set_used(frame);
	}

	mVA->bind();

	GLCall(glPatchParameteri(GL_PATCH_VERTICES, 1));
	GLCall(glDrawArrays(GL_PATCHES, 0, 1));

	mVA->unbind();
	mShader->unbind();
}
void SkCircleCurve::update_impl()
{
	update_annots();
	mNeed_update = false;
	circleData tosend = { basePlane()->to_worldPos(center_pos()), radius_val(), basePlane()->v(), basePlane()->w() };
	mVB->bind();
	mVB->set(&tosend, sizeof(circleData));
	mVB->unbind();
}