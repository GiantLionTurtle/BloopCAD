
#include "Collection.hpp"

Collection::Collection()
{

}

void Collection::draw(Camera_ptr cam, int frame, draw_type type /*= draw_type::ALL*/, bool on_required /*= false*/)
{
	if(exists()) { // Only draw if it exists and it is either visible or selected (if it is selected in the tree view for instance)
		if(!mInited) {
			init();
			mInited = true;
		}
		for(size_t i = 0; i < num_children(); ++i) {
			child(i)->draw(cam, frame, type, on_required);
		}
		mNeed_redraw = false;
	}
}

void Collection::update()
{
	if(!mInited) {
		init();
		mInited = true;
	}
	update_self();
	for(size_t i = 0; i < num_children(); ++i) {
		child(i)->update();
	}
	if(notif_on_update())
		notify_parent(UPDATED);
}