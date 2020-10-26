
#ifndef TEXTURE_HPP_
#define TEXTURE_HPP_

class texture {
protected:
	unsigned int mRendererID;
	unsigned char* mLocalBuffer;
	int mWidth, mHeight, mBPP;
public:
	texture();
	texture(int width, int height, unsigned int wrap_s, unsigned int wrap_t, unsigned int min_filter, unsigned int max_filter, unsigned int channels);
	~texture();

	virtual void bind(unsigned int slot = 0) const;
	virtual void unbind() const;

	unsigned int width() const { return mWidth; }
	unsigned int height() const { return mHeight; }

	unsigned int id() const { return mRendererID; }

	struct point {
		float x, y;
		float s, t;
	};
};

#endif
