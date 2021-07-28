
#ifndef SKSPRITE_HPP_
#define SKSPRITE_HPP_

#include "SkDrawable.hpp"
#include <geometry/geometry_2d/Point_abstr.hpp>
#include <constraintsSolver/expression.hpp>
#include <graphics_utils/Texture.hpp>

#include <map>
#include <string>

struct pos_offseted {
	glm::vec3 pos;
	glm::vec2 pixelOffset;
};
class SkSprite : public SkDrawable {
private:
	static bool kFisrstInst;
	static glm::vec3 kColorHovered;
	static std::map<std::string, std::shared_ptr<Texture>> kTextures; // map of textures to avoid reloading the same sprite multiple times

	std::string mTexturePath;
	std::shared_ptr<Texture> mTexture;
	glm::vec2 mDimensions;
	glm::vec2 mPixelOffset, mPos;   // A sprite has a position in the world as well
									// as a screen offset that stays constant regardless of zoom
	VertexArray* mVA;
	VertexBuffer* mVB;
	std::shared_ptr<Shader> mShader;
public:
	SkSprite(Geom3d::plane_abstr* basePlane_, glm::vec2 dims, std::string const& texturePath);
	~SkSprite();

	void init();
	void draw(Camera_ptr cam, int frame, draw_type type = draw_type::ALL);
	void update();
	
	SelectionPoint closest_2d(glm::vec2 planePos, Camera* cam, glm::vec2 cursorPos, int filter);
	// will have to find another way to drag then the geometry drag, I think it would be ridiculous to solve a system for this (the solve would be fast but still)
	DraggableSelectionPoint closest_2d_draggable(glm::vec2 planePos, Camera* cam, glm::vec2 cursorPos, int filter);
	int selection_rank() { return 1; }
	void move(glm::vec2 from, glm::vec2 to, glm::vec2 pixel_move);

	glm::vec2 pixelOffset() { return mPixelOffset; }
	void set_pixelOffset(glm::vec2 offset);

	glm::vec2 pos() { return mPos; }
	void set(glm::vec2 pos_);
};

#endif