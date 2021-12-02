
# BloopCAD
# Copyright (C) 2020-2021 BloopCorp

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

set(Graphics_utils_source_files
	GLCall.cpp
	Shader.cpp
	ShadersPool.cpp
	VertexBuffer.cpp
	VertexBufferLayout.cpp
	IndexBuffer.cpp
	VertexArray.cpp
	FrameBuffer.cpp
	Camera.cpp
	Texture.cpp
	Image.cpp
	TexturesPool.cpp
)

add_library(Graphics_utils ${Graphics_utils_source_files})

target_link_libraries(Graphics_utils
	Vendor
	Utils
)