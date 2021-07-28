
#include "Folder.hpp"

#include "entityView.hpp"


Folder::Folder(std::string const& name_)
{
	set_name(name_);
}

void Folder::set_handle(entityHandle* handle_)
{
	mHandle = handle_;
	for(auto ent : mDrawList) {
		if(ent->handle()) {
			delete ent->handle();
			ent->set_handle(nullptr);
		}
		ent->set_handle(new entityHandle(ent, mHandle->view(), mHandle));
	}
}
void Folder::add(Drawable* ent)
{
	if(ent) {
		need_redraw();
		ent->set_parent(this);
		mDrawList.push_back(ent);
		if(mHandle)
			ent->set_handle(new entityHandle(ent, mHandle->view(), mHandle));
	}
}

Drawable* Folder::get(size_t ind)
{
	if(ind <= mDrawList.size())
		return mDrawList[ind];
	return nullptr;
}