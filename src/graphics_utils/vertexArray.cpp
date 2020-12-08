
#include "GLCall.hpp"
#include "vertexArray.hpp"

vertexArray::vertexArray()
{
	GLCall(glGenVertexArrays(1, &mRendererID));
	int id_c;
	GLCall(glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &id_c));
}
vertexArray::~vertexArray()
{
	GLCall(glDeleteVertexArrays(1, &mRendererID));
}

void vertexArray::add_buffer(vertexBuffer const& vb, vertexBufferLayout const& layout)
{
	vb.bind();
	std::vector<vertexBufferProprety> const& propreties = layout.propreties();

	unsigned int offset = 0;
	for(unsigned int i = 0; i < propreties.size(); ++i) {
		vertexBufferProprety const& proprety = propreties[i];

		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, proprety.count, proprety.type, proprety.normalized, layout.stride(), (void*)(intptr_t)offset));
		offset += proprety.count * proprety.typeSize();
	}
	vb.unbind();
}

void vertexArray::bind() const
{
	GLCall(glBindVertexArray(mRendererID));
}
void vertexArray::unbind() const
{
#ifndef RELEASE_MODE
	GLCall(glBindVertexArray(0));
#endif
}
