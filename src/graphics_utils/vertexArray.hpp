
#ifndef VERTEXARRAY_HPP_
#define VERTEXARRAY_HPP_

#include "vertexBuffer.hpp"
#include "vertexBufferLayout.hpp"

class vertexArray {
private:
  unsigned int mRendererID;
public:
  vertexArray();
  ~vertexArray();

  void add_buffer(vertexBuffer const& vb, vertexBufferLayout const& layout);

  void bind() const;
  void unbind() const;

  unsigned int id() const { return mRendererID; }
};

#endif
