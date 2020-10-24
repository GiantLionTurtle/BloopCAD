
#ifndef VERTEXBUFFER_HPP_
#define VERTEXBUFFER_HPP_

class vertexBuffer {
private:
  unsigned int mRendererID;

  unsigned int mSize;
public:
  vertexBuffer(void const* data, unsigned int size_);
  ~vertexBuffer();

  void bind() const;
  void unbind() const;

  unsigned int size();
};

#endif
