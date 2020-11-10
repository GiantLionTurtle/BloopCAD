
// https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Text_Rendering_02

#include <cmath>
#include <algorithm>


#include "text.hpp"
#include <graphics_utils/GLCall.hpp>

#include <stb_image.h>

// struct character_info {
// 	float ax; // advance along x
// 	float ay; // advance along y

// 	float bw; // bitmap width
// 	float bh; // bitmap height

// 	float bl; // bitmap left
// 	float bt; // bitmap top

// 	float tx; // offset of character in the texture
// 	float ty;
// } char_info[128];

text::text(std::string const& str, float x, float y, float sx, float sy) :
	mText(str)
{
	if(FT_Init_FreeType(&ft)) {
		std::cerr << "Could not initi freetype library" << std::endl;
	}

	FT_Face face;
	if(FT_New_Face(ft, "ressources/textures/fonts/FreeSans.ttf", 0, &face)) {
		std::cerr << "Could not open font at \"" << "ressources/textures/fonts/FreeSans.ttf" << "\"" << std::endl;
	}

	mAtlas = std::shared_ptr<atlas>(new atlas(face, 48));

	mVB_layout.add_proprety_float(2); // Position
	mVB_layout.add_proprety_float(2); // Texture position

	point* pts = new point[4*str.size()];
	unsigned int* indices = new unsigned int[6*str.size()];
	int n = 0, in = 0;
	for(int i = 0; i < str.size(); ++i) {
		char c = str[i];
		float x2 = x + mAtlas->at(c).bl * sx;
		float y2 = -y - mAtlas->at(c).bt * sy;
		float w = mAtlas->at(c).bw * sx;
		float h = mAtlas->at(c).bh * sy;

		x += mAtlas->at(c).ax * sx;
		y += mAtlas->at(c).ay * sy;

		if(!w || !h)
			continue;

		pts[n++] = (point) {x2, 	-y2, 		mAtlas->at(c).tx, 												mAtlas->at(c).ty};
		pts[n++] = (point) {x2, 	-y2 - h,	mAtlas->at(c).tx, 												mAtlas->at(c).ty + mAtlas->at(c).bh / (float)mAtlas->height()};
		pts[n++] = (point) {x2 + w, -y2 - h, 	mAtlas->at(c).tx + mAtlas->at(c).bw / (float)mAtlas->width(), 	mAtlas->at(c).ty + mAtlas->at(c).bh / (float)mAtlas->height()};
		pts[n++] = (point) {x2 + w, -y2, 		mAtlas->at(c).tx + mAtlas->at(c).bw / (float)mAtlas->width(), 	mAtlas->at(c).ty};

		indices[in++] = n - 4;
		indices[in++] = n - 3;
		indices[in++] = n - 2;
		indices[in++] = n - 2;
		indices[in++] = n - 1;
		indices[in++] = n - 4;
	}

	mIB = new indexBuffer(&indices[0], 6*str.size());
	mVB = new vertexBuffer(pts, sizeof(point) * 4 * str.size());
	mVA = new vertexArray();
	mVA->bind();
	mVA->add_buffer(*mVB, mVB_layout);

	delete [] pts;
	delete [] indices;
}
text::~text()
{}

void text::bind(unsigned int slot /* = 0 */) const
{
	mVA->bind();
	mIB->bind();
	mAtlas->bind();
	// GLCall(glActiveTexture(GL_TEXTURE0+slot));
	// GLCall(glBindTexture(GL_TEXTURE_2D, mRendererID));
}
void text::unbind() const
{
	mVA->unbind();
	mIB->unbind();
	mAtlas->unbind();
// #ifndef RELEASE_MODE // legit in this context?
// 	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
// #endif
}

unsigned int text::count() const
{
	return mIB->count();
}

// unsigned int text::renderFont(std::string const& filePath, unsigned int pixelSize, FT_Library ft_, int& atlasWidth, int& atlasHeight)
// {
// 	FT_Face face;
// 	if(FT_New_Face(ft_, filePath.c_str(), 0, &face)) {
// 		std::cerr << "Could not open font at \"" << filePath << "\"" << std::endl;
// 	}

// 	FT_Set_Pixel_Sizes(face, 0, pixelSize);

// 	unsigned int textureID;
// 	GLCall(glGenTextures(1, &textureID));
// 	GLCall(glActiveTexture(GL_TEXTURE0));
// 	GLCall(glBindTexture(GL_TEXTURE_2D, textureID));


// 	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
// 	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
// 	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
// 	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

// 	GLCall(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));

// 	FT_GlyphSlot g = face->glyph;
// 	int w = 0;
// 	int h = 0;

// 	for(int i = 32; i < 128; i++) {
// 		if(FT_Load_Char(face, i, FT_LOAD_RENDER)) {
// 			fprintf(stderr, "Loading character %c failed!\n", i);
// 			continue;
// 		}

// 		w += g->bitmap.width;
// 		if(h < g->bitmap.rows)
// 			h = g->bitmap.rows;
// 	}

// 	/* you might as well save this value as it is needed later on */
// 	atlasWidth = w;
// 	atlasHeight = h;

// 	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, 0));

// 	int x = 0;

// 	for(int i = 32; i < 128; i++) {
// 		if(FT_Load_Char(face, i, FT_LOAD_RENDER))
// 			continue;
// 		x += g->bitmap.width;

// 		glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, g->bitmap.width, g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);

// 		char_info[i].ax = g->advance.x >> 6;
// 		char_info[i].ay = g->advance.y >> 6;

// 		char_info[i].bw = g->bitmap.width;
// 		char_info[i].bh = g->bitmap.rows;

// 		char_info[i].bl = g->bitmap_left;
// 		char_info[i].bt = g->bitmap_top;

// 		char_info[i].tx = (float)x / w;
// 	}

// 	return textureID;
// }