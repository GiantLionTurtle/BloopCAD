
#ifndef INDEXBUFFER_HPP_
#define INDEXBUFFER_HPP_

/*
	@class indexBuffer describes an abstraction of an openGL indices buffer
	@warning : Make sure all the functions are called within the same openGL context
*/
class indexBuffer {
private:
	unsigned int mRendererID; // The internal openGL id
	unsigned int mCount; // The NUMBER of indices contained
public:
	/*
		@function indexBuffer creates an indexBuffer object

		@param data : 	A pointer to the data assigned to the buffer, currently, only unsigned ints are supported
		@param count_ : The NUMBER of data points
	*/
	indexBuffer(unsigned int const* data, unsigned int count_);
	/*
		@function ~indexBuffer destroys the indexBuffer object and cleans it's openGL handle
	*/
	~indexBuffer();

	/*
		@function bind activates the indexBuffer inside openGL
	*/
	void bind() const;
	void unbind() const;
	/*
		@function unbind desactivates the indexBuffer inside openGL
	*/

	/*
		@function count 

		@return : The NUMBER of indices in the buffer
	*/
	unsigned int count() const { return mCount; };
	/*
		@function id

		@return : The internal openGL handle to the buffer
	*/
	unsigned int id() const { return mRendererID; }
};

#endif
