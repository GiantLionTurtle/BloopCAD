
#include "folder.hpp"

#include "entityView.hpp"


folder::folder(std::string const& name_, entity* parent_):
	entity(parent_)
{
	set_name(name_);
}

void folder::set_handle(entityHandle* handle_)
{
	mHandle = handle_;
	for_each([this](entity_ptr ent) {
		if(ent->handle()) {
			delete ent->handle();
			ent->set_handle(nullptr);
		}
		ent->set_handle(new entityHandle(ent, mHandle->view(), mHandle));
	});
}
void folder::add(entity_ptr ent)
{
	if(ent) {
		require_redraw();
		ent->set_parent(this);
		mChildren.push_back(ent);
		if(mHandle)
			ent->set_handle(new entityHandle(ent, mHandle->view(), mHandle));
	}
}

void folder::for_each(std::function<void (entity_ptr)> func)
{
	for(entity_ptr ent : mChildren) {
		func(ent);
	}
}