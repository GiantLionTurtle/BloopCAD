
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

sketchLine::sketchLine(sketchPoint_ptr ptA, sketchPoint_ptr ptB, sketch* parent_sk, bool immovable/* = false*/):
	sketchEntity(parent_sk->basePlane(), 2),
	mA(ptA),
	mB(ptB)
{
	parent_sk->add_geometry(mA);
	parent_sk->add_geometry(mB);
	// mA->set_parent(this);
	// mB->set_parent(this);
	if(immovable) {
		mA->set_constant();
		mB->set_constant();
	}
	init();
}
sketchLine::sketchLine(glm::vec2 ptA, glm::vec2 ptB, sketch* parent_sk, bool immovable/* = false*/):
	sketchEntity(parent_sk->basePlane(), 2),
	mA(sketchPoint_ptr(new sketchPoint(ptA, parent_sk->basePlane(), immovable))),
	mB(sketchPoint_ptr(new sketchPoint(ptB, parent_sk->basePlane(), immovable)))
{
	parent_sk->add_geometry(mA);
	parent_sk->add_geometry(mB);
	// mA->set_parent(this);
	// mB->set_parent(this);
	if(immovable) {
		mA->set_constant();
		mB->set_constant();
	}
	init();
}

void sketchLine::init()
{
	dirX = std::make_shared<expression_substr_funky>(mA->var()->expr()->x, mB->var()->expr()->x, 0.0);
	dirY = std::make_shared<expression_substr_funky>(mA->var()->expr()->y, mB->var()->expr()->y, 0.0);
	dir = std::make_shared<expressionVector2>(dirX, dirY);

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
}

void sketchLine::for_each(std::function<void (entity_ptr)> func)
{
	// func(mA);
	// func(mB);
}

void sketchLine::move(glm::vec2 from, glm::vec2 to)
{
	glm::vec2 d = to-from;
	mA->set(mA->pos() + d);
	mB->set(mB->pos() + d);
	set_require_VBUpdate();
}

bool sketchLine::in_selection_range(glm::vec2 planepos, camera_ptr cam, glm::vec2 cursor)
{
	glm::vec4 onscreen_ndc = cam->mvp()	* glm::vec4(mBasePlane->to_worldPos(closest_to_point(planepos)), 1.0f);
	glm::vec2 onscreen(	map(onscreen_ndc.x / onscreen_ndc.w, -1.0f, 1.0f, 0.0f, cam->viewport().x),
						map(onscreen_ndc.y / onscreen_ndc.w, -1.0f, 1.0f, cam->viewport().y, 0.0f));
	if(glm::distance2(onscreen, cursor) < 50)
		return true;
	return false;
}

void sketchLine::notify_childUpdate()
{
	tangibleEntity::notify_childUpdate();
	glm::vec2 dir = mA->pos() - mB->pos();
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
	glm::vec3 tmp[2] = { mBasePlane->to_worldPos(mA->pos()), mBasePlane->to_worldPos(mB->pos()) };
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

// float sketchLine::selection_depth(camera_ptr cam, glm::vec2 cursor_pos)
// {
// 	glm::vec3 inter = mBasePlane->line_intersection(cam->pos(), cam->cast_ray(cursor_pos));
// 	glm::vec2 on_plane = mBasePlane->to_planePos(inter);
// 	glm::vec4 closest_screen(mBasePlane->to_worldPos(closest_to_point(on_plane)), 1.0f);
// 	closest_screen = cam->mvp() * closest_screen;
// 	closest_screen /= closest_screen.w;
// 	glm::vec2 on_screen_pix(map(closest_screen.x, -1.0f, 1.0f, 0.0f, cam->viewport().x), 
// 							map(closest_screen.y, -1.0f, 1.0f, cam->viewport().y, 0.0f));
// 	if(glm::length2(cursor_pos - on_screen_pix) < 50) {
// 		return glm::length(cam->pos() - inter);
// 	}
// 	return -1.0f;
// }
