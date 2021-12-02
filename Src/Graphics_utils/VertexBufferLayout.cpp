
// BloopCAD
// Copyright (C) 2020-2021 BloopCorp

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "VertexBufferLayout.hpp"
#include "GLCall.hpp"

VertexBufferLayout::VertexBufferLayout():
  mStride(0)
{

}

unsigned int VertexBufferProprety::typeSize() const
{
  switch(type) {
    case GL_FLOAT:        return 4;
    case GL_UNSIGNED_INT: return 4;
    case GL_UNSIGNED_BYTE:return 1;
  };

  LOG_WARNING("Unknown type.");
  return 0;
}

void VertexBufferLayout::add_proprety_float(unsigned int count)
{
  mPropreties.push_back({ count, GL_FLOAT, GL_FALSE }); // Normalized is false, not an option
  mStride += sizeof(float) * count; // The stride expands with each addition of proprety
}
void VertexBufferLayout::add_proprety_uint(unsigned int count)
{
  mPropreties.push_back({ count, GL_UNSIGNED_INT, GL_FALSE }); // Normalized is false, not an option
  mStride += sizeof(unsigned int) * count; // The stride expands with each addition of proprety
}
void VertexBufferLayout::add_proprety_uchar(unsigned int count)
{
  mPropreties.push_back({ count, GL_UNSIGNED_BYTE, GL_FALSE }); // Normalized is false, not an option
  mStride += sizeof(unsigned char) * count; // The stride expands with each addition of proprety
}
