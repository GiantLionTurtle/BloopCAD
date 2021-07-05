
#ifndef FOLDER_HPP_
#define FOLDER_HPP_

#include "Drawable.hpp"

class folder;
using folder_ptr = std::shared_ptr<folder>;

class folder : public Drawable {
protected:
	std::vector<Drawable_ptr> mChildren;
public:
	folder(std::string const& name_ = "folder", Drawable* parent_ = nullptr);

	virtual void set_handle(entityHandle* handle_);
	
	void add(Drawable_ptr elem);

	virtual void for_each(std::function<void (Drawable_ptr)> func);
};

#endif