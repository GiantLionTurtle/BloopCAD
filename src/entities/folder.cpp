
#include "folder.hpp"

#include "entityView.hpp"


folder::folder(std::string const& name_)
{
	set_name(name_);
}

void folder::set_handle(entityHandle* handle_)
{
	mHandle = handle_;
	for(int i = 0; i < mChildren.size(); ++i) {
		if(mChildren[i]->handle())
			delete mChildren[i]->handle();
		mChildren[i]->set_handle(new entityHandle(mChildren[i], mHandle->view(), mHandle));
	}
}
void folder::add(entity_ptr ent)
{
	entity::add(ent);
	if(ent && mHandle) {
		ent->set_handle(new entityHandle(ent, mHandle->view(), mHandle));
	}
}