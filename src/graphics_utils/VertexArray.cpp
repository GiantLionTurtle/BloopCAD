
#include "GLCall.hpp"
#include "VertexArray.hpp"

VertexArray::VertexArray()
{
	// Create the vertex array
	GLCall(glGenVertexArrays(1, &mRendererID));
}
VertexArray::~VertexArray()
{
	// Delete the vertex array
	GLCall(glDeleteVertexArrays(1, &mRendererID));
}

void VertexArray::add_buffer(VertexBuffer const& vb, VertexBufferLayout const& layout)
{
	vb.bind(); // Bind the vertex buffer to add it to the vertex array

	// Set the desired layout for the buffer within the vertex array
	std::vector<VertexBufferProprety> const& propreties = layout.propreties();
	unsigned int offset = 0;
	for(unsigned int i = 0; i < propreties.size(); ++i) {
		VertexBufferProprety const& proprety = propreties[i]; // Current proprety

		GLCall(glEnableVertexAttribArray(i)); // Enable said proprety
		GLCall(glVertexAttribPointer(i, proprety.count, proprety.type, proprety.normalized, layout.stride(), (void*)(intptr_t)offset)); // Describe proprety
		offset += proprety.count * proprety.typeSize(); // Increase our position in the vertex array's index
	}

	// Clean up
	vb.unbind();
}

void VertexArray::bind() const
{
	GLCall(glBindVertexArray(mRendererID));
}
void VertexArray::unbind() const
{
#ifndef RELEASE_MODE // No unecesary binding in release, but practical for debugging purposes
	GLCall(glBindVertexArray(0));
#endif
}
