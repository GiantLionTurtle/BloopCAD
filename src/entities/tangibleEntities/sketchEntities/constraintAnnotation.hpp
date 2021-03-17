
#ifndef CONSTRAINTANNOTATION_HPP_
#define CONSTRAINTANNOTATION_HPP_

#include <entities/tangibleEntities/sketchEntities/sketchEntity.hpp>
#include <graphics_utils/textures/texture.hpp>

#include <map>

class constraint_abstract;

class constraintAnnotation : public sketchEntity {
private:
	std::map<int, std::shared_ptr<texture>> kTextures;

	std::shared_ptr<texture> mTexture;
	int mWidth, mHeight;

	glm::vec2 mPos, mWorldOffset, mPixelOffset;

	std::shared_ptr<constraint_abstract> mConstraint;
	std::shared_ptr<constraintAnnotation> mTwin;
public:
	constraintAnnotation(geom_3d::plane_abstr_ptr basePlane_, std::shared_ptr<constraint_abstract> constr);
	~constraintAnnotation();

	void init();

	void print(int depth = 0);

	glm::vec2 pos();
	void set_pos(glm::vec2 const& pos_);
	void set_pixelOffset(glm::vec2 const& offset);

	void update_VB();

	void set_twin(std::shared_ptr<constraintAnnotation> tw) { mTwin = tw; }
	std::shared_ptr<constraintAnnotation> twin() const { return mTwin; }
protected:
	void draw_impl(camera_ptr cam, int frame);

	void exists_impl(bool ex);
	void hidden_impl(bool hid);
};

#endif