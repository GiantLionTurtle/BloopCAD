
#include "sketchCircle.hpp"

#include <graphics_utils/GLCall.hpp>
#include <graphics_utils/ShadersPool.hpp>
#include <utils/mathUtils.hpp>
#include <utils/preferences.hpp>
#include <geometry/geometry_2d/line_abstr.hpp>

float sketchCircle::kSelDist2 = 0.0f;
bool sketchCircle::kFisrstInst = true;
glm::vec3 sketchCircle::kColor = glm::vec3(0.0); 
glm::vec3 sketchCircle::kColorHovered = glm::vec3(0.0);
glm::vec3 sketchCircle::kColorSelected = glm::vec3(0.0);

sketchCircle::sketchCircle(glm::vec2 center_, float radius_, geom_3d::plane_abstr_ptr basePlane_):
	sketchGeometry(basePlane_, types::CIRCLE),
	mCenter(sketchPoint_ptr(new sketchPoint(center_, basePlane_))),
	mRadius(expression_var::make(radius_))
{
	mCenter->set_parent(this);
	// init();
}
sketchCircle::~sketchCircle()
{

}

void sketchCircle::init()
{
	mRequire_VBUpdate = false;
	set_name("sketchCircle");
	init_buffers(); // Set all the vertices to the right values
	
	mVB = std::shared_ptr<VertexBuffer>(new VertexBuffer(mVertices, sizeof(glm::vec3) * CIRCLE_RES)); // Upload the vertices
	mVA = std::shared_ptr<VertexArray>(new VertexArray());

	VertexBufferLayout layout;
	layout.add_proprety_float(3);
	mVA->bind();
	mVA->add_buffer(*mVB.get(), layout);
	mIB = std::shared_ptr<IndexBuffer>(new IndexBuffer(mIndices, 2*CIRCLE_RES));

	// // Create the Shader
	// mShader = ShadersPool::get_instance().get("circle");
	// if(!mShader) {
	// 	mShader = Shader::fromFiles_ptr("resources/shaders/circleShader.vert", "resources/shaders/circleShader.frag");
	// 	ShadersPool::get_instance().add("circle", mShader);
	// }

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

void sketchCircle::print(int depth)
{
	for(int i = 0; i < depth; ++i) {
		std::cout<<"\t";
	}
	std::cout<<name()<<"["<<mRadius->eval()<<"]\n";
	
	mCenter->print(depth+1);
}

void sketchCircle::move(glm::vec2 from, glm::vec2 to, glm::vec2 pixel_move)
{
	if(!mCenter->dragged())
		set_radius(glm::length(to - mCenter->pos()));
}

bool sketchCircle::in_selection_range(glm::vec2 planepos, Camera_ptr cam, glm::vec2 cursor)
{
	glm::vec4 onscreen_ndc = cam->mvp()	* glm::vec4(mBasePlane->to_worldPos(closest_to_point(planepos)), 1.0f);
	glm::vec2 onscreen(	map(onscreen_ndc.x / onscreen_ndc.w, -1.0f, 1.0f, 0.0f, cam->viewport().x),
						map(onscreen_ndc.y / onscreen_ndc.w, -1.0f, 1.0f, cam->viewport().y, 0.0f));
	if(glm::distance2(onscreen, cursor) < 25)
		return true;
	return false;
}
bool sketchCircle::in_selection_range(glm::vec2 a, glm::vec2 b, bool contained)
{
	if(contained) {
		glm::vec2 centerpos = posCenter();
		double r = valRadius();
		return a.x >= centerpos.x + r && a.y <= centerpos.y - r && b.x <= centerpos.x - r && b.y >= centerpos.y + r;
	} else {
		glm::vec2 centerpos = posCenter();
		double r = valRadius();
		geom_2d::simple_line 	up(b, glm::vec2(a.x, b.y)),
								down(glm::vec2(b.x, a.y), a),
								// right(a, glm::vec2(a.x, b.y)),
								left(glm::vec2(b.x, a.y), b);
		return intersects(&up) || intersects(&down) /*|| intersects(&right)*/ || intersects(&left) || in_selection_range(a, b, true);		
	}
	
	return false;
}

void sketchCircle::set_constant()
{
	mRadius->set_is_coef(true);
}
void sketchCircle::set_tmpConstant(bool const_)
{
	if(const_) {
		mRadius->set_as_coef();
	} else {
		mRadius->set_as_var();
	}
}

void sketchCircle::for_each(std::function<void (Drawable_ptr)> func)
{
	func(mCenter);
	sketchGeometry::for_each(func);
}
void sketchCircle::for_each(std::function<void (sketchEntity_ptr)> func)
{
	func(mCenter);
	sketchGeometry::for_each(func);
}
void sketchCircle::for_each(std::function<void(sketchGeometry_ptr geom)> func)
{
	func(mCenter);
}

void sketchCircle::set_radius(float newval)
{
	mRadius->set(newval);
	set_require_VBUpdate();
}

entityPosSnapshot_ptr sketchCircle::posSnapshot()
{
	return std::make_shared<circleSnapshot>(shared_from_this());
}

void sketchCircle::update_VB()
{
	if(!mInited)
		return;
	init_buffers();
	mVB->bind();
	mVB->set(mVertices, sizeof(glm::vec3) * CIRCLE_RES);
	mVB->unbind();
}

void sketchCircle::set_exists_vars(bool ex)
{
	mRadius->set_exists(ex);
	mCenter->set_exists_vars(ex);
}

void sketchCircle::draw_impl(Camera_ptr cam, int frame)
{
	mShader->bind();
	glm::vec4 color = glm::vec4(kColor, 1.0f);
	if(selected()) {
		color = glm::vec4(kColorSelected, 1.0f);
	} else if(hovered()) {
		color = glm::vec4(kColorHovered, 1.0f);
	}
	
	mShader->setUniform4f("u_Color", color);
	
	if(mShader->lastUsed() != frame) {
		mShader->setUniformMat4f("u_MVP", cam->mvp());
		mShader->set_used(frame);
	}
	
	mVA->bind();
	mIB->bind();

	GLCall(glDrawElements(GL_LINES, mIB->count(), GL_UNSIGNED_INT, nullptr)); // Actual draw call

	// Clean up
	mIB->unbind();
	mVA->unbind();
	mShader->unbind();
}

void sketchCircle::post_set_behavior()
{
	mRequire_VBUpdate = true;
	set_need_redraw();
}

void sketchCircle::init_buffers()
{
	// // Set all vertices in anti-clockwise fashion
	// mVertices[0] = mCenter->pos_val() + glm::vec3( mRadius->val(),  mRadius->val(), 0.0f);
	// mVertices[1] = mCenter->pos_val() + glm::vec3(-mRadius->val(),  mRadius->val(), 0.0f);
	// mVertices[2] = mCenter->pos_val() + glm::vec3(-mRadius->val(), -mRadius->val(), 0.0f);
	// mVertices[3] = mCenter->pos_val() + glm::vec3( mRadius->val(), -mRadius->val(), 0.0f);

	// mIndices[0] = 0;
	// mIndices[1] = 1;
	// mIndices[2] = 2; // First triangle
	// mIndices[3] = 2;
	// mIndices[4] = 3;
	// mIndices[5] = 0; // Second triangle

	for(int i = 0; i < CIRCLE_RES; ++i) {
		float t = (float)i / CIRCLE_RES;
		mVertices[i] = mBasePlane->to_worldPos(at(t));
		mIndices[2*i] = i;
		mIndices[2*i+1] = i+1;
	}
	mIndices[2*CIRCLE_RES-1] = 0;
}


circleSnapshot::circleSnapshot(sketchCircle_ptr pt): 
	mCircle(pt) 
{
	x = mCircle->center()->x()->eval();
	y = mCircle->center()->y()->eval();
	r = mCircle->radius()->eval();
}
circleSnapshot::circleSnapshot(sketchCircle_ptr source, sketchCircle_ptr target): 
	mCircle(target) 
{
	x = source->center()->x()->eval();
	y = source->center()->y()->eval();
	r = source->radius()->eval();
}

void circleSnapshot::apply()
{
	mCircle->center()->x()->set(x);
	mCircle->center()->y()->set(y);
	mCircle->radius()->set(r);
	mCircle->set_require_VBUpdate();
}