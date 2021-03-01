
#include "sketchPoint.hpp"

#include <utils/errorLogger.hpp>
#include <graphics_utils/shadersPool.hpp>
#include <utils/mathUtils.hpp>
#include <graphics_utils/GLCall.hpp>
#include <utils/preferences.hpp>

float sketchPoint::kSelDist2 = 0.0f;
bool sketchPoint::kFisrstInst = true;
glm::vec3 sketchPoint::kColor = glm::vec3(0.0); 
glm::vec3 sketchPoint::kColorHovered = glm::vec3(0.0);

sketchPoint::sketchPoint(glm::vec2 pos_2d, geom_3d::plane_abstr_ptr basePlane_, bool immovable/* = false*/):
	sketchEntity(basePlane_, types::POINT),
	mX(expression_variable::make(pos_2d.x)),
	mY(expression_variable::make(pos_2d.y))
{
	if(immovable)
		set_constant();
	init();
}
sketchPoint::sketchPoint(variable_ptr x_, variable_ptr y_, geom_3d::plane_abstr_ptr basePlane_, bool immovable/* = false*/):
	sketchEntity(basePlane_, types::POINT),
	mX(x_),
	mY(y_)
{
	if(immovable) 
		set_constant();
	init();
}

void sketchPoint::init()
{
	mRequire_VBUpdate = false;
	mVA = std::shared_ptr<vertexArray>(new vertexArray());
	vertexBufferLayout layout;
	layout.add_proprety_float(3);
	mVA->bind();

	glm::vec3 pos_tmp = mBasePlane->to_worldPos(pos());
	mVB = std::shared_ptr<vertexBuffer>(new vertexBuffer(&pos_tmp[0], sizeof(glm::vec3)));
	mVA->add_buffer(*mVB.get(), layout);

	mShader = shadersPool::get_instance().get("point");
	if(!mShader) {
		mShader = shader::fromFiles_ptr({
		{"resources/shaders/pointShader.vert", GL_VERTEX_SHADER},
		{"resources/shaders/pointShader.geom", GL_GEOMETRY_SHADER}, 
		{"resources/shaders/pointShader.frag", GL_FRAGMENT_SHADER}}); // Geometry shader is needed because point is expanded on the gpu
		shadersPool::get_instance().add("point", mShader);
	}
	if(kFisrstInst) {
		kSelDist2 = preferences::get_instance().get_float("seldistpoint2");
		preferences::get_instance().add_callback("seldistpoint2", std::function<void(float)>([this](float val) { kSelDist2 = val; }));
		kColor = preferences::get_instance().get_vec3("sketchEntityColor");
		preferences::get_instance().add_callback("sketchEntityColor", std::function<void(glm::vec3)>([this](glm::vec3 val) { kColor = val; }));
		kColorHovered = preferences::get_instance().get_vec3("sketchEntityColorHovered");
		preferences::get_instance().add_callback("sketchEntityColorHovered", std::function<void(glm::vec3)>([this](glm::vec3 val) { kColorHovered = val; }));

		kFisrstInst = false;
	}
	set_name("sketchPoint");
}

void sketchPoint::print(int depth)
{
	for(int i = 0; i < depth; ++i) {
		std::cout<<"\t";
	}
	std::cout<<name()<<"("<<mX->eval()<<",  "<<mY->eval()<<")\n";
}

void sketchPoint::move(glm::vec2 from, glm::vec2 to)
{
	set(to);
}

bool sketchPoint::in_selection_range(glm::vec2 planepos, camera_ptr cam, glm::vec2 cursor)
{
	glm::vec4 onscreen_ndc = cam->mvp()	* glm::vec4(mBasePlane->to_worldPos(closest_to_point(planepos)), 1.0f);
	glm::vec2 onscreen(	map(onscreen_ndc.x / onscreen_ndc.w, -1.0f, 1.0f, 0.0f, cam->viewport().x),
						map(onscreen_ndc.y / onscreen_ndc.w, -1.0f, 1.0f, cam->viewport().y, 0.0f));
	if(glm::distance2(onscreen, cursor) < 50)
		return true;
	return false;
}

void sketchPoint::set(glm::vec2 p)
{
	mX->set(p.x);
	mY->set(p.y);
	set_require_VBUpdate();
}

void sketchPoint::set_constant()
{
	mX->set_is_coef(true);
	mY->set_is_coef(true);
}
void sketchPoint::set_tmpConstant(bool const_)
{
	if(const_) {
		mX->set_as_coef();
		mY->set_as_coef();
	} else {
		mX->set_as_var();
		mY->set_as_var();
	}
}

void sketchPoint::update_VB()
{
	mVB->bind();
	glm::vec3 pos_tmp = mBasePlane->to_worldPos(pos());
	mVB->set(&pos_tmp[0], sizeof(glm::vec3));
	mVB->unbind();
	set_require_redraw();
	if(mParent)
            mParent->notify_childUpdate();
	mRequire_VBUpdate = false;
}

void sketchPoint::draw_impl(camera_ptr cam, int frame)
{
	if(mRequire_VBUpdate) // very sketch
		update_VB();
		
	mShader->bind();
	glm::vec4 color = glm::vec4(kColor, 1.0f);
	if(hovered()) {
		color = glm::vec4(kColorHovered, 1.0f);
	} else if(selected()) {
		color = glm::vec4(0.01f, 0.70f, 0.99f, 1.0f);
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

// float sketchPoint::selection_depth(camera_ptr cam, glm::vec2 cursor_pos)
// {
// 	glm::vec4 screenPos = cam->mvp() * glm::vec4(mBasePlane->to_worldPos(pos()), 1.0f);
// 	glm::vec2 screenPos_2d(	map(screenPos.x / screenPos.w, -1.0f, 1.0f, 0.0f, cam->viewport().x),
// 							map(screenPos.y / screenPos.w, -1.0f, 1.0f, cam->viewport().y, 0.0f));
	
// 	if(glm::length(screenPos_2d - cursor_pos) < 5) {
// 		return glm::length(cam->pos() - mBasePlane->line_intersection(cam->pos(), cam->cast_ray(cursor_pos)));
// 	}
// 	return -1.0f;
// }

void sketchPoint::post_set_behavior()
{
	set_require_redraw();
	mRequire_VBUpdate = true;
}