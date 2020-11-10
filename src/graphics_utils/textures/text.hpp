
#ifndef TEXT_HPP_
#define TEXT_HPP_

#include "texture.hpp"
#include "atlas.hpp"
#include <graphics_utils/vertexBufferLayout.hpp>
#include <graphics_utils/vertexBuffer.hpp>
#include <graphics_utils/indexBuffer.hpp>
#include <graphics_utils/vertexArray.hpp>

#include <unordered_map>
#include <memory>
#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H  


// struct font {
// 	unsigned int atlas_width, atlas_height;
// 	unsigned int pixelSize;
// 	unsigned int rendererID;
// };

class text : public texture {
private:
	std::string mText;

	FT_Library ft;

	vertexBuffer* mVB;
	vertexArray* mVA;
	indexBuffer* mIB;
	vertexBufferLayout mVB_layout;

	std::shared_ptr<atlas> mAtlas;
public:
	text(std::string const& str, float x, float y, float sx, float sy);
	~text();

	virtual void bind(unsigned int slot = 0) const;
	virtual void unbind() const;

	unsigned int count() const;

	struct point {
		float x, y;
		float s, t;
	};

private:
	static unsigned int renderFont(std::string const& filePath, unsigned int pixelSize, FT_Library ft_, int& atlasWidth, int& atlasHeight);
};

#endif