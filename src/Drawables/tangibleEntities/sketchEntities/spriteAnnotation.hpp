#ifndef SPRITEANNOTATION_HPP_
#define SPRITEANNOTATION_HPP_

#include <Drawables/tangibleEntities/sketchEntities/sketchEntity.hpp>
#include <constraintsSolver/expression.hpp>
#include <graphics_utils//Texture.hpp>

#include <map>
#include <string>

struct pos_offseted {
	glm::vec3 pos;
	glm::vec2 pixelOffset;
};

class spriteAnnotation : public sketchEntity {
private:
	static bool kFisrstInst;
	static glm::vec3 kColorHovered;
	static std::map<std::string, std::shared_ptr<Texture>> kTextures; // map of textures to avoid reloading the same sprite multiple times

	std::string mTexturePath;
	std::shared_ptr<Texture> mTexture;
	glm::vec2 mDimensions;
	glm::vec2 mPixelOffset, mPos;   // A sprite has a position in the world as well
																	// as a screen offset that stays constant regardless of zoom
public:
	spriteAnnotation(geom_3d::plane_abstr_ptr basePlane_, glm::vec2 dims, std::string const& texturePath);
	~spriteAnnotation();

	void init();

	void move(glm::vec2 from, glm::vec2 to, glm::vec2 pixel_move);

	bool in_selection_range(glm::vec2 planepos, Camera_ptr cam, glm::vec2 cursor);

	glm::vec2 pos() { return mPos; }
	void set_pos(glm::vec2 pos_);
	glm::vec2 pixelOffset() { return mPixelOffset; }
	void set_pixelOffset(glm::vec2 offset);

	void update_VB();

	int selection_rank() { return 1; }
protected:
	void draw_impl(Camera_ptr cam, int frame);
};



#endif
