
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
glm::vec3 sketchPoint::kColorSelected = glm::vec3(0.0);

sketchPoint::sketchPoint(glm::vec2 pos_2d, geom_3d::plane_abstr_ptr basePlane_, bool immovable/* = false*/):
	sketchGeometry(basePlane_, types::POINT),
	mX(expression_var::make(pos_2d.x)),
	mY(expression_var::make(pos_2d.y))
{
	if(immovable)
		set_constant();
	// init();
}
sketchPoint::sketchPoint(var_ptr x_, var_ptr y_, geom_3d::plane_abstr_ptr basePlane_, bool immovable/* = false*/):
	sketchGeometry(basePlane_, types::POINT),
	mX(x_),
	mY(y_)
{
	if(immovable) 
		set_constant();
	// init();
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
		kColorSelected = preferences::get_instance().get_vec3("sketchEntityColorSelected");
		preferences::get_instance().add_callback("sketchEntityColorSelected", std::function<void(glm::vec3)>([this](glm::vec3 val) { kColorSelected = val; }));

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

void sketchPoint::move(glm::vec2 from, glm::vec2 to, glm::vec2 pixel_move)
{
	set(pos() + to-from);
}

bool sketchPoint::in_selection_range(glm::vec2 planepos, camera_ptr cam, glm::vec2 cursor)
{
	glm::vec2 closest_vec2 = closest_to_point(planepos);
	glm::vec4 pos_vec4 = glm::vec4(mBasePlane->to_worldPos(closest_vec2), 1.0f);
	glm::vec4 onscreen_ndc = cam->mvp()	* pos_vec4;
	glm::vec2 onscreen(	map(onscreen_ndc.x / onscreen_ndc.w, -1.0f, 1.0f, 0.0f, cam->viewport().x),
						map(onscreen_ndc.y / onscreen_ndc.w, -1.0f, 1.0f, cam->viewport().y, 0.0f));
	if(glm::distance2(onscreen, cursor) < 50)
		return true;
	return false;
}
bool sketchPoint::in_selection_range(glm::vec2 a, glm::vec2 b, bool contained)
{
	if(a.x >= mX->eval() && b.x <= mX->eval() && a.y <= mY->eval() && b.y >= mY->eval())
		return true;
	return false;
}

void sketchPoint::set(glm::vec2 p)
{
	mX->set(p.x);
	mY->set(p.y);
	set_require_VBUpdate();
}
entityPosSnapshot_ptr sketchPoint::posSnapshot()
{
	return std::make_shared<pointSnapshot>(shared_from_this());
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
	if(!mInited)
		return;
	mVB->bind();
	glm::vec3 pos_tmp = mBasePlane->to_worldPos(pos());
	mVB->set(&pos_tmp[0], sizeof(glm::vec3));
	mVB->unbind();
	mRequire_VBUpdate = false;

	for(auto annot : mFloatingAnnots) {
		annot->set_pos(pos());
	}
}

glm::vec2 sketchPoint::annotation_pixelOffset(int ind)
{
	int level = ind / 6;
	float angle = (float)(ind % 6) / 6.0f * M_PI * 2.0 + M_PI_2;
	return glm::vec2(std::cos(angle) * (float)(ind+1) * 25.0f, std::sin(angle) * (float)(ind+1) * 25.0f);
}
void sketchPoint::position_floatingAnnotation(std::shared_ptr<spriteAnnotation> annot)
{
	annot->set_pos(pos());
	annot->set_pixelOffset(annotation_pixelOffset(mFloatingAnnots.size()));
}
void sketchPoint::set_exists_vars(bool ex)
{
	mX->set_exists(ex);
	mY->set_exists(ex);
}

void sketchPoint::draw_impl(camera_ptr cam, int frame)
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


void sketchPoint::post_set_behavior()
{
	set_require_redraw();
	mRequire_VBUpdate = true;
}

void sketchPoint::set_dragged_impl(bool drag)
{
	mX->set_dragged(drag);
	mY->set_dragged(drag);
}


pointSnapshot::pointSnapshot(sketchPoint_ptr pt): 
	mPoint(pt)
{
	x = mPoint->x()->eval();
	y = mPoint->y()->eval();
}
pointSnapshot::pointSnapshot(sketchPoint_ptr source, sketchPoint_ptr target): 
	mPoint(target)
{
	x = source->x()->eval();
	y = source->y()->eval();
}

void pointSnapshot::apply()
{
	mPoint->x()->set(x);
	mPoint->y()->set(y);
	mPoint->set_require_VBUpdate();
}