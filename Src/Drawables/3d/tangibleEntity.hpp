
#ifndef GEOMETRICENTITY_HPP_
#define GEOMETRICENTITY_HPP_

#include <Drawables/Base/Drawable.hpp>

class tangibleEntity : public Drawable {
protected:
	std::shared_ptr<VertexBuffer> 	mVB; // The vertex buffer to contain the vertices on the gpu
	std::shared_ptr<VertexArray> 	mVA; // The vertex array to contain the attributes on the gpu
	std::shared_ptr<IndexBuffer>    mIB; // The index buffer to contain vertex handles on the gpu
	Shader* 		mShader; // Shader to draw the entity
	bool mNeed_VBUpdate;
public:
	tangibleEntity(): 
		mVB(nullptr), 
		mVA(nullptr), 
		mIB(nullptr),
		mShader(nullptr), 
		mNeed_VBUpdate(false) {}
	virtual ~tangibleEntity() {}

	virtual void init() = 0;

	void draw(Camera* cam, int frame, draw_type type = draw_type::ALL, bool on_required = false)
	{
		// This is not exactly duplicate logic from what is in entity.cpp
		if(!mInited) {
			init();
			mInited = true;
		}
		if(mNeed_VBUpdate)
			update_VB();
		Drawable::draw(cam, frame, type, on_required);
	}

	virtual void notify(int msg)
	{
		if(msg == UPDATED && mParent)
			mParent->notify(msg);
		set_require_VBUpdate();
	}

	virtual void update_VB() = 0;

	void set_require_VBUpdate() { mNeed_VBUpdate = true; set_need_redraw(); notify_parent(UPDATED); }
	bool require_VBUpdate() const { return mNeed_VBUpdate; }
protected:
	virtual void graphicUpdate_impl() { set_require_VBUpdate(); }
};

#endif