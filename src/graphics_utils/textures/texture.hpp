
#ifndef TEXTURE_HPP_
#define TEXTURE_HPP_

/*
	@class texture describes an abstraction of an openGL texture
*/
class texture {
protected:
	unsigned int mRendererID; // The handle of the openGL texture
	unsigned char* mLocalBuffer; // A local buffer containing the texture data
	int mWidth, mHeight, mBPP; // Metrics 
public:
	/*
		@function texture creates an empty texture object
	*/
	texture();
	/*
		@function texture creates a parametrized texture object

		@param width : 		The width in pixels of the texture
		@param height : 	The height in pixels of the texture
		@param wrap_s : 	The wrap type on the s (x) axis of the texture
		@param wrap_t : 	The wrap type on the t (y) axis of the texture
		@param min_filter : The minifiying function of the texture
		@param mag_filter : The magnifiying function of the texture
		@param channels : 	The number of color channels of the texture
	*/
	texture(int width, int height, unsigned int wrap_s, unsigned int wrap_t, unsigned int min_filter, unsigned int mag_filter, unsigned int channels);
	/*
		@function ~texture destroys a texture object and handle deallocation in openGL
	*/
	virtual ~texture();

	/*
		@function bind activates the texture in openGL at a specific slot

		@param slot [defaults to 0] : The texture slot in which the texture will be bound
	*/
	virtual void bind(unsigned int slot = 0) const;
	/*
		@function unbind desactivate the texture in opengl
	*/
	virtual void unbind() const;

	/*
		@function width

		@return : The width of the texture in pixels
	*/
	unsigned int width() const { return mWidth; }
	/*
		@function height 

		@return : The height of the texture in pixels
	*/
	unsigned int height() const { return mHeight; }

	/*
		@function id

		@return : The openGL handle of the texture
	*/
	unsigned int id() const { return mRendererID; }
};

#endif
