
#include "sketchLine.hpp"
#include "sketchPoint.hpp"

#include <graphics_utils/shadersPool.hpp>
#include <utils/errorLogger.hpp>
#include <utils/mathUtils.hpp>
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


sketchLine::sketchLine(line_abstr const& baseLine, geom_3d::plane_abstr_ptr basePlane_, bool immovable /*= false*/):
	sketchEntity(basePlane_),
	line_abstr(baseLine)
{
	if(immovable) {
		mA->set_constant();
		mB->set_constant();
	}

	dirX = std::make_shared<expression_substr_funky>(mA->var()->expr()->x, mB->var()->expr()->x, 0.0);
	dirY = std::make_shared<expression_substr_funky>(mA->var()->expr()->y, mB->var()->expr()->y, 0.0);
	dir = std::make_shared<expressionVector2>(dirX, dirY);

	init();
}

void sketchLine::init()
{
	sketchPoint_ptr A_as_drawable = std::dynamic_pointer_cast<sketchPoint>(mA);
	sketchPoint_ptr B_as_drawable = std::dynamic_pointer_cast<sketchPoint>(mB);
	if(!A_as_drawable) {
		A_as_drawable = std::shared_ptr<sketchPoint>(new sketchPoint(mA->var(), mBasePlane));
		mA = A_as_drawable;
	}
	if(!B_as_drawable) {
		B_as_drawable = std::shared_ptr<sketchPoint>(new sketchPoint(mB->var(), mBasePlane));
		mB = B_as_drawable;
	}

	set_name("line");
	mVA = std::shared_ptr<vertexArray>(new vertexArray());
	vertexBufferLayout layout;
	layout.add_proprety_float(3);
	mVA->bind();

	glm::vec3 tmp[2] = { mBasePlane->to_worldPos(mA->vec()), mBasePlane->to_worldPos(mB->vec()) };
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
	
	add(A_as_drawable);
	add(B_as_drawable);
}

void sketchLine::notify_childUpdate()
{
	tangibleEntity::notify_childUpdate();
	glm::vec2 dir = mA->vec() - mB->vec();
	if(glm::length2(dir) == 0.0f) {
		dirX->funk(true);
		dirY->funk(true);
	} else {
		dirX->funk(false);
		dirY->funk(false);
		dirX->set_funk(dir.x);
		dirY->set_funk(dir.y);
	}
}

void sketchLine::update_VB()
{
	mRequire_VBUpdate = false;
	glm::vec3 tmp[2] = { mBasePlane->to_worldPos(mA->vec()), mBasePlane->to_worldPos(mB->vec()) };
	mVB->bind();
	mVB->set(&tmp[0], sizeof(glm::vec3) * 2);
	mVB->unbind();
}

std::vector<variable_ptr> sketchLine::variables()
{
	return { mA->var()->x, mA->var()->y, mB->var()->x, mB->var()->y };
}

subEquationsSystem sketchLine::coincidence()
{
	variable_ptr t = std::make_shared<variable>(0.5f);
	return { 	{ t }, 
				{ mA->var()->x->expr() - t->expr() * (mA->var()->x->expr() - mB->var()->x->expr()),
				  mA->var()->y->expr() - t->expr() * (mA->var()->y->expr() - mB->var()->y->expr()) }};
}
subEquationsSystem_vec sketchLine::direction()
{
	return { {}, dir };
}
std::vector<subEquationsSystem> sketchLine::verticality()
{
	return { { {}, { mA->var()->x->expr() } },
			 { {}, { mB->var()->x->expr() } }};
}
std::vector<subEquationsSystem> sketchLine::horizontality()
{
	return { { {}, { mA->var()->y->expr() } },
			 { {}, { mB->var()->y->expr() } }};
}

void sketchLine::draw_impl(camera_ptr cam, int frame)
{
	if(mRequire_VBUpdate)
		update_VB();
	mShader->bind();
	glm::vec4 color = glm::vec4(mColor, 1.0f);
	if(hovered()) {
		color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	} else if(selected()) {
		color = glm::vec4(0.01f, 0.70f, 0.99f, 1.0f);
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

float sketchLine::selection_depth(camera_ptr cam, glm::vec2 cursor_pos)
{
	glm::vec4 screenPos_a = cam->mvp() * glm::vec4(mA->vec(), 0.0f, 1.0f);
	glm::vec2 screenPos_a_2d(	map(screenPos_a.x / screenPos_a.w, -1.0f, 1.0f, 0.0f, cam->viewport().x),
								map(screenPos_a.y / screenPos_a.w, -1.0f, 1.0f, cam->viewport().y, 0.0f));

	glm::vec4 screenPos_b = cam->mvp() * glm::vec4(mB->vec(), 0.0f, 1.0f);
	glm::vec2 screenPos_b_2d(	map(screenPos_b.x / screenPos_b.w, -1.0f, 1.0f, 0.0f, cam->viewport().x),
								map(screenPos_b.y / screenPos_b.w, -1.0f, 1.0f, cam->viewport().y, 0.0f));

	line_abstr screen_line(screenPos_a_2d, screenPos_b_2d);
	float line_point = 0;
	if(glm::distance2(screen_line.closest_to_point(cursor_pos), cursor_pos) < 25) {
		return glm::length(cam->pos() - mBasePlane->line_intersection(cam->pos(), cam->cast_ray(cursor_pos)));		
	}
	return -1.0f;
}
