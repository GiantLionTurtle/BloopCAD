
#ifndef ATLAS_HPP_
#define ATLAS_HPP_

// https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Text_Rendering_02

#include "texture.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H 

class atlas : public texture {
public:
	struct char_info {
		float ax; // advance along x
		float ay; // advance along y

		float bw; // bitmap width
		float bh; // bitmap height

		float bl; // bitmap left
		float bt; // bitmap top

		float tx; // x offset of the character in the texture
		float ty; // y offset of the character in the texture
	};
private:
	char_info* mCharInfos;

	const int max_width = 1024;
public:
	atlas(FT_Face face, int fontHeight);
	~atlas();

	char_info& operator[](char c);
	char_info& at(char c);
};

#endif