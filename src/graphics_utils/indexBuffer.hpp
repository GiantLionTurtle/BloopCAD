
#ifndef INDEXBUFFER_HPP_
#define INDEXBUFFER_HPP_

class indexBuffer {
private:
  unsigned int mRendererID;
  unsigned int mCount;
public:
  indexBuffer(unsigned int const* data, unsigned int count_);
  ~indexBuffer();

  void bind() const;
  void unbind() const;
  unsigned int count() const { return mCount; };
};

#endif
