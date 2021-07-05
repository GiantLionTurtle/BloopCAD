
#ifndef COLLECTION_HPP_
#define COLLECTION_HPP_

#include "Drawable.hpp"

class Collection : public Drawable {
private:

public:
	Collection();

	void draw(Camera_ptr cam, int frame, draw_type type = draw_type::ALL, bool on_required = false) = 0;
		
	void update();
	virtual void update_self() {}

	virtual size_t num_children() = 0;
	virtual Drawable* child(size_t ind) = 0;
};

#endif