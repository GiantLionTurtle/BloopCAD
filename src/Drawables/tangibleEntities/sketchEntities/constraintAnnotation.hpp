
#ifndef CONSTRAINTANNOTATION_HPP_
#define CONSTRAINTANNOTATION_HPP_

#include <Drawables/tangibleEntities/sketchEntities/sketchEntity.hpp>
#include "entityPosSnapshot_abstract.hpp"
#include <graphics_utils//Texture.hpp>

#include <map>

class constraint_abstract;
class constraintAnnotation;
using constraintAnnotation_ptr = std::shared_ptr<constraintAnnotation>;

class constraintAnnotation : public sketchEntity {
private:
	static bool kFisrstInst;
	static glm::vec3 kColorHovered;
	static std::map<int, std::shared_ptr<Texture>> kTextures;

	std::shared_ptr<Texture> mTexture;
	int mWidth, mHeight;

	glm::vec2 mPos, mWorldOffset, mPixelOffset;

	std::shared_ptr<constraint_abstract> mConstraint;
	constraintAnnotation_ptr mTwin;
public:
	constraintAnnotation(geom_3d::plane_abstr_ptr basePlane_, std::shared_ptr<constraint_abstract> constr);
	~constraintAnnotation();

	void init();

	void move(glm::vec2 from, glm::vec2 to);

	void print(int depth = 0);

	bool in_selection_range(glm::vec2 planepos, Camera_ptr cam, glm::vec2 cursor);

	glm::vec2 pos();
	void set_pos(glm::vec2 const& pos_);
	void set_pixelOffset(glm::vec2 const& offset);

	void update_VB();

	void set_twin(constraintAnnotation_ptr tw) { mTwin = tw; }
	constraintAnnotation_ptr twin() const { return mTwin; }

	int selection_rank() { return 1; }
protected:
	void draw_impl(Camera_ptr cam, int frame);

	void exists_impl(bool ex);
	// void hidden_impl(bool hid);
	void hover_impl(bool hov);
};

#endif