
#ifndef GEOMETRICENTITY_HPP_
#define GEOMETRICENTITY_HPP_

#include <entities/entity.hpp>

class tangibleEntity : public entity {
protected:
	std::shared_ptr<vertexBuffer> 	mVB; // The vertex buffer to contain the vertices on the gpu
	std::shared_ptr<vertexArray> 	mVA; // The vertex array to contain the attributes on the gpu
    std::shared_ptr<indexBuffer>    mIB; // The index buffer to contain vertex handles on the gpu
	std::shared_ptr<shader> 		mShader; // Shader to draw the entity
    bool mRequire_VBUpdate;
public:
    tangibleEntity(): 
        mVB(nullptr), 
        mVA(nullptr), 
        mIB(nullptr),
        mShader(nullptr), 
        mRequire_VBUpdate(false) {}
    virtual ~tangibleEntity() {}

    virtual void init() = 0;

    virtual void notify_childUpdate()
    {
        if(mParent)
            mParent->notify_childUpdate();
        update_VB();
    }

    virtual void update_VB() = 0;

    bool require_VBUpdate() const { return mRequire_VBUpdate; }
protected:
    virtual void update_impl() { update_VB(); }
};

#endif