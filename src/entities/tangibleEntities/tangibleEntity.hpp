
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

	void draw(camera_ptr cam, int frame, draw_type type = draw_type::ALL, bool on_required = false)
	{
		// This is not exactly duplicate logic from what is in entity.cpp
		if(!mInited) {
			init();
			mInited = true;
		}
		if(mRequire_VBUpdate)
			update_VB();
		entity::draw(cam, frame, type, on_required);
	}

	virtual void notify(int msg)
	{
		if(msg == UPDATED && mParent)
			mParent->notify(msg);
		set_require_VBUpdate();
	}

	virtual void update_VB() = 0;

	void set_require_VBUpdate() { mRequire_VBUpdate = true; set_require_redraw(); notify_parent(UPDATED); }
	bool require_VBUpdate() const { return mRequire_VBUpdate; }
protected:
	virtual void update_impl() { set_require_VBUpdate(); }
};

#endif