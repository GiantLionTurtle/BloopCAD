
#include "atlas.hpp"
#include <graphics_utils/GLCall.hpp>

atlas::atlas(FT_Face face, int fontHeight)
{
	mCharInfos = new char_info[128];//face->num_glyphs];
	memset(mCharInfos, 0, sizeof(char_info) * 128);

	FT_Set_Pixel_Sizes(face, 0, fontHeight);

	GLCall(glGenTextures(1, &mRendererID));
	GLCall(glActiveTexture(GL_TEXTURE0));
	GLCall(glBindTexture(GL_TEXTURE_2D, mRendererID));


	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLCall(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));

	FT_GlyphSlot g = face->glyph;

	int row_width = 0;
	int row_height = 0;

	for(int i = 32; i < 128; i++) {
		if(FT_Load_Char(face, i, FT_LOAD_RENDER)) {
			std::cerr << "Error while loading of character " << (char)i << std::endl;
			continue;
		}
		if(row_width + g->bitmap.width + 1 > max_width) {
			mWidth = mWidth > row_width ? mWidth : row_width;
			mHeight += row_height;

			row_width = 0;
			row_height = 0;
		}
		row_width += g->bitmap.width + 1;
		row_height = row_height > g->bitmap.rows ? row_height : g->bitmap.rows;
	}

	mWidth = mWidth > row_width ? mWidth : row_width;
	mHeight += row_height;

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, mWidth, mHeight, 0, GL_RED, GL_UNSIGNED_BYTE, 0));

	int row_x = 0;
	int row_y = 0;
	row_height = 0;
	for(int i = 32; i < 128; i++) {
		if(FT_Load_Char(face, i, FT_LOAD_RENDER)) {
			std::cerr << "Error while loading of character " << (char)i << std::endl;
			continue;
		}
		
		if(row_x + g->bitmap.width + 1 > max_width) {
			row_x = 0;
			row_y += row_height;
			row_height = 0;
		}

		glTexSubImage2D(GL_TEXTURE_2D, 0, row_x, row_y, g->bitmap.width, g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);

		mCharInfos[i].ax = g->advance.x >> 6;
		mCharInfos[i].ay = g->advance.y >> 6;

		mCharInfos[i].bw = g->bitmap.width;
		mCharInfos[i].bh = g->bitmap.rows;

		mCharInfos[i].bl = g->bitmap_left;
		mCharInfos[i].bt = g->bitmap_top;

		mCharInfos[i].tx = (float)row_x / mWidth;
		mCharInfos[i].ty = (float)row_y / mHeight;
		row_x += g->bitmap.width + 1;
		row_height = row_height > g->bitmap.rows ? row_height : g->bitmap.rows;
	}

	std::cout << "Generated a " << mWidth << " x " << mHeight << " atlas" << std::endl;
}

atlas::~atlas()
{
	delete [] mCharInfos;
}

atlas::char_info& atlas::operator[](char c)
{
	return mCharInfos[c];
}

atlas::char_info& atlas::at(char c)
{
	return mCharInfos[c];
}