
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
		if(std::get<1>(mChildren[i])->handle())
			delete std::get<1>(mChildren[i])->handle();
		std::get<1>(mChildren[i])->set_handle(new entityHandle(std::get<1>(mChildren[i]), mHandle->view(), mHandle));
	}
}
void folder::add(std::shared_ptr<entity> ent)
{
	entity::add(ent);
	if(ent && mHandle) {
		ent->set_handle(new entityHandle(ent, mHandle->view(), mHandle));
	}
}