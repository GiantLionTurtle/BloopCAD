
#include "constraintAnnotation.hpp"

#include <constraintsSolver/constraint.hpp>
#include <graphics_utils/textures/image.hpp>
#include <graphics_utils/shadersPool.hpp>
#include <graphics_utils/GLCall.hpp>
#include <utils/errorLogger.hpp>
#include <utils/mathUtils.hpp>
#include <utils/preferences.hpp>

bool constraintAnnotation::kFisrstInst = true;
glm::vec3 constraintAnnotation::kColorHovered = glm::vec3(0.0f);
std::map<int, std::shared_ptr<texture>> constraintAnnotation::kTextures = {};

constraintAnnotation::constraintAnnotation(geom_3d::plane_abstr_ptr basePlane_, std::shared_ptr<constraint_abstract> constr):
	sketchEntity(basePlane_, -1),
	mTexture(nullptr),
	mWidth(20),
	mHeight(20),
	mPixelOffset(0.0f),
	mPos(0.0f),
	mWorldOffset(0.0f),
	mConstraint(constr),
	mTwin(nullptr)
{
	if(!mConstraint) { // I have no idea why but it crashes without the curly braces.. probably a problem higher up.
		LOG_ERROR("Null constraint");
	}
}
constraintAnnotation::~constraintAnnotation()
{

}

void constraintAnnotation::init()
{
	int tp = mConstraint->type();
	if(kTextures.find(tp) == kTextures.end()) {
		switch(tp) {
		case constraint_abstract::COINCIDENCE:
			set_name("coincidence_annot");
			mTexture = std::make_shared<image>("resources/textures/images/icons/sketch/constraints/coincidence.png");
			break;
		case constraint_abstract::HORIZONTALITY:
			set_name("horizontality_annot");
			mTexture = std::make_shared<image>("resources/textures/images/icons/sketch/constraints/horizontality.png");
			break;
		case constraint_abstract::VERTICALITY:
			set_name("verticality_annot");
			mTexture = std::make_shared<image>("resources/textures/images/icons/sketch/constraints/verticality.png");
			break;
		case constraint_abstract::PARALLELISM:
			set_name("parallelism_annot");
			mTexture = std::make_shared<image>("resources/textures/images/icons/sketch/constraints/parallelism.png");
			break;
		case constraint_abstract::PERPENDICULARITY:
			set_name("perpendicularity_annot");
			mTexture = std::make_shared<image>("resources/textures/images/icons/sketch/constraints/perpendicularity.png");
			break;
		default:
			set_name("unknown_annot");
			mTexture = std::make_shared<image>("resources/textures/images/icons/icon.png");
			break;
		}
		kTextures[tp] = mTexture;
	} else {
		mTexture = kTextures[tp];
	}

	mRequire_VBUpdate = false;
	mVA = std::make_shared<vertexArray>();
	vertexBufferLayout layout;
	layout.add_proprety_float(3);
	mVA->bind();

	glm::vec3 tmpPos = mBasePlane->to_worldPos(mPos + mWorldOffset);
	mVB = std::make_shared<vertexBuffer>(&tmpPos[0], sizeof(glm::vec3));
	mVA->add_buffer(*mVB, layout);

	mShader = shadersPool::get_instance().get("fixedSizeTexturedQuad");
	if(!mShader) {
		mShader = shader::fromFiles_ptr({
		{"resources/shaders/pointShader.vert", GL_VERTEX_SHADER},
		{"resources/shaders/fixedSizeQuad.geom", GL_GEOMETRY_SHADER}, 
		{"resources/shaders/annotationShader.frag", GL_FRAGMENT_SHADER}}); // Geometry shader is needed because point is expanded on the gpu
		shadersPool::get_instance().add("fixedSizeTexturedQuad", mShader);
	}
	if(kFisrstInst) {
		kColorHovered = preferences::get_instance().get_vec3("sketchEntityColorHovered");
		preferences::get_instance().add_callback("sketchEntityColorHovered", std::function<void(glm::vec3)>([this](glm::vec3 val) { kColorHovered = val; }));
		kFisrstInst = false;
	}
}

void constraintAnnotation::print(int depth)
{
	for(int i = 0; i < depth; ++i) {
		std::cout<<"\t";
	}
	std::cout<<name()<<"[("<<mPos.x<<",  "<<mPos.y<<") ; ("<<mPixelOffset.x<<",  "<<mPixelOffset.y<<")]\n";
}
bool constraintAnnotation::in_selection_range(glm::vec2 planepos, camera_ptr cam, glm::vec2 cursor)
{
	glm::vec4 onscreen_ndc = cam->mvp()	* glm::vec4(mBasePlane->to_worldPos(mPos + mWorldOffset), 1.0f);
	glm::vec2 onscreen(	map(onscreen_ndc.x / onscreen_ndc.w, -1.0f, 1.0f, 0.0f, cam->viewport().x),
						map(onscreen_ndc.y / onscreen_ndc.w, -1.0f, 1.0f, cam->viewport().y, 0.0f));
	onscreen += glm::vec2(mPixelOffset.x, -mPixelOffset.y);

	if(cursor.x < onscreen.x + mWidth/2 && cursor.x > onscreen.x - mWidth/2 &&
	   cursor.y < onscreen.y + mHeight/2 && cursor.y > onscreen.y - mHeight/2) {
		return true;
	}
	return false;
}
glm::vec2 constraintAnnotation::pos()
{
	return mPos;
}
void constraintAnnotation::set_pos(glm::vec2 const& pos_)
{
	mPos = pos_;
	set_require_VBUpdate();
}

void constraintAnnotation::set_pixelOffset(glm::vec2 const& offset)
{
	mPixelOffset = offset;
}

void constraintAnnotation::update_VB()
{
	if(!mInited)
		return;
	mRequire_VBUpdate = false;
	glm::vec3 tmpPos = mBasePlane->to_worldPos(mPos + mWorldOffset);
	mVB->bind();
	mVB->set(&tmpPos[0], sizeof(glm::vec3) * 1);
	mVB->unbind();
}

void constraintAnnotation::draw_impl(camera_ptr cam, int frame)
{
	if(mRequire_VBUpdate) // very sketch
		update_VB();
		
	mShader->bind();
	glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f);
	if(hovered()) 
		color = kColorHovered;

	mShader->setUniform3f("u_Color", color);
	mShader->setUniform1f("u_Width", 40);
	mShader->setUniform1f("u_Height", 40);
	mShader->setUniform2f("u_PixelOffset", mPixelOffset);

	if(mShader->lastUsed() != frame) {
		mShader->setUniformMat4f("u_MVP", cam->mvp());
		mShader->setUniform2f("u_Viewport", cam->viewport());
		mShader->set_used(frame);
	}

	mTexture->bind();
	mVA->bind();

	GLCall(glDrawArrays(GL_POINTS, 0, 1)); // No indexing needed, a line only has one vertex

	mVA->unbind();
	mShader->unbind();
}

void constraintAnnotation::exists_impl(bool ex)
{
	if(mTwin && mTwin->exists() != ex)
		mTwin->set_exists(ex);
	sketchEntity::exists_impl(ex);
}
// void constraintAnnotation::hidden_impl(bool hid)
// {
// 	if(mTwin && mTwin->exists() && mTwin->hidden() != hid)
// 		mTwin->set_hidden(hid);
// }
void constraintAnnotation::hover_impl(bool hov)
{
	if(mTwin && mTwin->exists() && mTwin->hovered() != hov) {
		mTwin->set_hidden(!hov);
		mTwin->set_hover(hov);
	}
}