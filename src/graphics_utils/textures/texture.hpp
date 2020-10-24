
#ifndef TEXTURE_HPP_
#define TEXTURE_HPP_

class texture {
protected:
	unsigned int mRendererID;
	unsigned char* mLocalBuffer;
	int mWidth, mHeight, mBPP;
public:
	texture();
	~texture();

	virtual void bind(unsigned int slot = 0) const;
	virtual void unbind() const;

	unsigned int width() const { return mWidth; }
	unsigned int height() const {return mHeight; }

	struct point {
		float x, y;
		float s, t;
	};
};

#endif
