
#include "sketchLine.hpp"
#include "sketchPoint.hpp"

#include <graphics_utils/shadersPool.hpp>
#include <utils/errorLogger.hpp>
#include <utils/mathUtils.hpp>
#include <utils/preferences.hpp>
#include <graphics_utils/GLCall.hpp>

expression_substr_funky::expression_substr_funky(expression_ptr left, expression_ptr right, double funkval):
	binary_expression(left, right),
	mFunk(false),
	mFunkval(funkval)
{
	mOp = operationType::SUBSTR;
}

double expression_substr_funky::eval()
{
	if(!mFunk)
		return mLeft->eval() - mRight->eval();
	return mFunkval;
}
expression_ptr expression_substr_funky::derivative()
{
	return mLeft->derivative() - mRight->derivative();
}

std::string expression_substr_funky::to_string()
{
	return "(" + mLeft->to_string() + " - " + mRight->to_string() + ")";
}

float sketchLine::kSelDist2 = 0.0f;
bool sketchLine::kFisrstInst = true;
glm::vec3 sketchLine::kColor = glm::vec3(0.0); 
glm::vec3 sketchLine::kColorHovered = glm::vec3(0.0);
glm::vec3 sketchLine::kColorSelected = glm::vec3(0.0);

sketchLine::sketchLine(sketchPoint_ptr ptA, sketchPoint_ptr ptB, geom_3d::plane_abstr_ptr basePlane_, bool immovable/* = false*/):
	sketchGeometry(basePlane_,  types::LINE),
	mA(ptA),
	mB(ptB)
{
	mA->set_parent(this);
	mB->set_parent(this);
	if(immovable) 
		set_constant();
	// init();
}
sketchLine::sketchLine(glm::vec2 ptA, glm::vec2 ptB, geom_3d::plane_abstr_ptr basePlane_, bool immovable/* = false*/):
	sketchGeometry(basePlane_, types::LINE),
	mA(sketchPoint_ptr(new sketchPoint(ptA, basePlane_, immovable))),
	mB(sketchPoint_ptr(new sketchPoint(ptB, basePlane_, immovable)))
{
	mA->set_parent(this);
	mB->set_parent(this);
	if(immovable) 
		set_constant();
	// init();
}

void sketchLine::init()
{
	// dirX = std::make_shared<expression_substr_funky>(mA->var()->expr()->x, mB->var()->expr()->x, 0.0);
	// dirY = std::make_shared<expression_substr_funky>(mA->var()->expr()->y, mB->var()->expr()->y, 0.0);
	// dir = std::make_shared<expressionVector2>(dirX, dirY);
	mRequire_VBUpdate = false;
	
	mLength2 = (pow(mA->x()-mB->x(), 2.0) + pow(mA->y()-mB->y(), 2.0));

	set_name("line");
	mVA = std::shared_ptr<vertexArray>(new vertexArray());
	vertexBufferLayout layout;
	layout.add_proprety_float(3);
	mVA->bind();

	glm::vec3 tmp[2] = { mBasePlane->to_worldPos(mA->pos()), mBasePlane->to_worldPos(mB->pos()) };
	mVB = std::shared_ptr<vertexBuffer>(new vertexBuffer(&tmp[0], sizeof(glm::vec3) * 2));
	mVA->add_buffer(*mVB.get(), layout);

	mShader = shadersPool::get_instance().get("line");
	if(!mShader) {
		mShader = shader::fromFiles_ptr({
		{"resources/shaders/lineShader.vert", GL_VERTEX_SHADER},
		{"resources/shaders/lineShader.geom", GL_GEOMETRY_SHADER}, 
		{"resources/shaders/lineShader.frag", GL_FRAGMENT_SHADER}}); // Geometry shader is needed because line is expanded on the gpu
		shadersPool::get_instance().add("line", mShader);
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

void sketchLine::print(int depth)
{
	for(int i = 0; i < depth; ++i) {
		std::cout<<"\t";
	}
	std::cout<<name()<<"[("<<mA->x()->eval()<<",  "<<mA->y()->eval()<<") ; ("<<mB->x()->eval()<<",  "<<mB->y()->eval()<<")]\n";
}

void sketchLine::for_each(std::function<void (entity_ptr)> func)
{
	func(mA);
	func(mB);
	sketchGeometry::for_each(func);
}
void sketchLine::for_each(std::function<void(sketchEntity_ptr geom)> func)
{
	func(mA);
	func(mB);
	sketchGeometry::for_each(func);
}

void sketchLine::move(glm::vec2 from, glm::vec2 to)
{
	glm::vec2 d = to-from;
	if(!mA->dragged())
		mA->set(mA->pos() + d);
	if(!mB->dragged())
		mB->set(mB->pos() + d);
	set_require_VBUpdate();
}

bool sketchLine::in_selection_range(glm::vec2 planepos, camera_ptr cam, glm::vec2 cursor)
{
	glm::vec4 onscreen_ndc = cam->mvp()	* glm::vec4(mBasePlane->to_worldPos(closest_to_point(planepos)), 1.0f);
	glm::vec2 onscreen(	map(onscreen_ndc.x / onscreen_ndc.w, -1.0f, 1.0f, 0.0f, cam->viewport().x),
						map(onscreen_ndc.y / onscreen_ndc.w, -1.0f, 1.0f, cam->viewport().y, 0.0f));
	if(glm::distance2(onscreen, cursor) < kSelDist2)
		return true;
	return false;
}
bool sketchLine::in_selection_range(glm::vec2 a, glm::vec2 b, bool contained)
{
	if(contained) {
		return mA->in_selection_range(a, b, true) && mB->in_selection_range(a, b, true);
	} else {
		geom_2d::simple_line diag_1(a, b), diag_2(glm::vec2(a.x, b.y), glm::vec2(b.x, a.y));
		return mA->in_selection_range(a, b, true) || mB->in_selection_range(a, b, true) ||
		intersects(&diag_1) || intersects(&diag_2);
	}
	return false;
}

void sketchLine::set_constant()
{
	mA->set_constant();
	mB->set_constant();
}
void sketchLine::set_tmpConstant(bool const_)
{
	mA->set_tmpConstant(const_);
	mB->set_tmpConstant(const_);
}

sketchLine_ptr sketchLine::clone()
{
	return std::make_shared<sketchLine>(mA->pos(), mB->pos(), mBasePlane);
}

void sketchLine::update_VB()
{
	if(!mInited)
		return;
	mRequire_VBUpdate = false;
	glm::vec3 tmp[2] = { mBasePlane->to_worldPos(mA->pos()), mBasePlane->to_worldPos(mB->pos()) };
	mVB->bind();
	mVB->set(&tmp[0], sizeof(glm::vec3) * 2);
	mVB->unbind();

	for(int i = 0; i < mAnnotations.size(); ++i) {
		mAnnotations[i]->set_pos((mA->pos() + mB->pos()) / 2.0f);
		mAnnotations[i]->set_pixelOffset(annotation_pixelOffset(i));
	}
}

void sketchLine::on_added_constraintAnnotation()
{
	mAnnotations.back()->set_pos((mA->pos() + mB->pos()) / 2.0f);
}

glm::vec2 sketchLine::annotation_pixelOffset(int ind)
{
	glm::vec2 dir = glm::normalize(mA->pos() - mB->pos());
	glm::vec2 normal = glm::cross(glm::vec3(dir, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	int line_side = ind % 2 == 0 ? 1 : -1;
	bool icon_dir = line_side == -1 ? ind % 4 ? -1 : 1 : ind % 3 ? -1 : 1;
	float dir_offset = line_side == 1 ? ind : ind - 1;
	return dir * 100.0f * (float)icon_dir * dir_offset + normal * 150.0f * (float)line_side;
}

void sketchLine::set_exists_vars(bool ex)
{
	mA->set_exists_vars(ex);
	mB->set_exists_vars(ex);
}

void sketchLine::draw_impl(camera_ptr cam, int frame)
{
	if(mRequire_VBUpdate)
		update_VB();
	mShader->bind();
	glm::vec4 color = glm::vec4(kColor, 1.0f);
	if(selected()) {
		color = glm::vec4(kColorSelected, 1.0f);
	} else if(hovered()) {
		color = glm::vec4(kColorHovered, 1.0f);
	}

	mShader->setUniform4f("u_Color", color);
	mShader->setUniform1f("u_LineWidth", 5);
	mShader->setUniform1f("u_Feather", 0.6);

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