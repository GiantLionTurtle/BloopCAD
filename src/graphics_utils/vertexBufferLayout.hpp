
#ifndef VERTEXBUFFERLAYOUT_HPP_
#define VERTEXBUFFERLAYOUT_HPP_

#include <vector>

struct vertexBufferProprety {
  unsigned int count;
  unsigned int type;
  unsigned int normalized;

  unsigned int typeSize() const;
};

class vertexBufferLayout {
private:
  std::vector<vertexBufferProprety> mPropreties;
  unsigned int mStride;
public:
  vertexBufferLayout():
    mStride(0)
  {}

  //template<typename T>
  //void add_proprety(unsigned int count) { static_assert(false); }

  //template<>
  void add_proprety_float(unsigned int count);
  void add_proprety_uint(unsigned int count);
  void add_proprety_uchar(unsigned int count);

  std::vector<vertexBufferProprety> const& propreties() const { return mPropreties; };
  unsigned int stride() const { return mStride; }
};

#endif