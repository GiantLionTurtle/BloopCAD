
#ifndef CONSTRAINTANNOTATION_HPP_
#define CONSTRAINTANNOTATION_HPP_

#include <entities/tangibleEntities/sketchEntities/sketchEntity.hpp>
#include <graphics_utils/textures/texture.hpp>

#include <map>

class constraintAnnotation : public sketchEntity {
private:
	std::map<int, std::shared_ptr<texture>> kTextures;

	int mType;
	std::shared_ptr<texture> mTexture;
	int mWidth, mHeight;

	glm::vec2 mPos, mWorldOffset, mPixelOffset;
public:
	constraintAnnotation(geom_3d::plane_abstr_ptr basePlane_, int constraintType);
	~constraintAnnotation();

	void init();

	glm::vec2 pos();
	void set_pos(glm::vec2 const& pos_);
	void set_pixelOffset(glm::vec2 const& offset);

	void update_VB();
protected:
	void draw_impl(camera_ptr cam, int frame);
};

#endif