
#ifndef VERTEXBUFFER_HPP_
#define VERTEXBUFFER_HPP_

/*
	@class vertexBuffer describes an abstraction of openGL's vertex buffer
	@warning : Make sure that all functions are called whithin the same openGL context
*/
class vertexBuffer {
private:
	unsigned int mRendererID; // The openGL handle to the buffer
	unsigned int mSize; // The size in bytes of the buffer
public:
	/*
		@function vertexBuffer creates a vertexBuffer object

		@param data : 	The data of the vertexBuffer, it can be of any types
		@param size_ : 	The size in bytes of the buffer
	*/
	vertexBuffer(void const* data, unsigned int size_);
	/*
		@function ~vertexBuffer destroys a vertexBuffer object and cleanly handles the deallocation in openGL
	*/
	~vertexBuffer();

	/*
		@function bind activates the buffer in openGL so that it can be used
	*/
	void bind() const;
	/*
		@function unbind desactivates the buffer in openGL so that it can't be used anymore
	*/
	void unbind() const;

	/*
		@function size

		@return : The size in bytes of the buffer
	*/
	unsigned int size() const { return mSize; }
	/*
		@function id

		@return : The openGL handle of the buffer
	*/
	unsigned int id() const { return mRendererID; }

	void set(void const* data, unsigned int size_);
};

#endif
