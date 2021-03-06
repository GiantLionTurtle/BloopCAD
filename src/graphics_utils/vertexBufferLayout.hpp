
#ifndef VERTEXBUFFERLAYOUT_HPP_
#define VERTEXBUFFERLAYOUT_HPP_

#include <vector>

/*
	@struct vertexBufferProprety describes a single proprety of a vertex (position, color, etc..)
*/
struct vertexBufferProprety {
	unsigned int count; // Number of element in the proprety
	unsigned int type; // Type of the elements (float, int, etc..)
	unsigned int normalized; // Whether ot not the proprety is normalized

	/*
		@function typeSize

		@return : The size in bytes of the type of the proprety
	*/
	unsigned int typeSize() const;
};

/*
	@class vertexBufferLayout describes the way propreties are laid out in a vertex buffer
*/
class vertexBufferLayout {
private:
	std::vector<vertexBufferProprety> mPropreties; // All the propreties of the layout
	unsigned int mStride; // The total size in bytes of a vertex with all the propreties
public:
	/*
		@function vertexBufferLayout creates an empty layout
	*/
	vertexBufferLayout();

	/*
		@function add_proprety_float adds a proprety of type float to the layout

		@param count : The number of element in the proprety
	*/
	void add_proprety_float(unsigned int count);
	/*
		@function add_proprety_uint adds a proprety of type unsigned int to the layout

		@param count : The number of element in the proprety
	*/	
	void add_proprety_uint(unsigned int count);
	/*
		@function add_proprety_uchar adds a proprety of type unsigned char to the layout

		@param count : The number of element in the proprety
	*/	
	void add_proprety_uchar(unsigned int count);

	/*
		@function propreties 

		@return : All the propreties of the layout
	*/
	std::vector<vertexBufferProprety> const& propreties() const { return mPropreties; };
	/*
		@function stride 

		@return : The total size of a single vertex with all the propreties
	*/
	unsigned int stride() const { return mStride; }
};

#endif