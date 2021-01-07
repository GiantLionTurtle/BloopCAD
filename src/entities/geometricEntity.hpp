
#ifndef GEOMETRICENTITY_HPP_
#define GEOMETRICENTITY_HPP_

#include "entity.hpp"

class geometricEntity : public entity {
protected:
	std::shared_ptr<vertexBuffer> 	mVB; // The vertex buffer to contain the vertices on the gpu
	std::shared_ptr<vertexArray> 	mVA; // The vertex array to contain the attributes on the gpu
    std::shared_ptr<indexBuffer>    mIB; // The index buffer to contain vertex handles on the gpu
	std::shared_ptr<shader> 		mShader, mSelectionShader; // Both shaders to draw the point and it's selection 
    bool mRequire_VBUpdate;
public:
    geometricEntity(): 
        mVB(nullptr), 
        mVA(nullptr), 
        mIB(nullptr),
        mShader(nullptr), 
        mSelectionShader(nullptr),
        mRequire_VBUpdate(false) {}

    virtual void update_VB() = 0;
};

#endif