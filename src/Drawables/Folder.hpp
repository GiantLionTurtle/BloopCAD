
#ifndef FOLDER_HPP_
#define FOLDER_HPP_

#include "Collection.hpp"

class Folder;
using Folder_ptr = std::shared_ptr<Folder>;

class Folder : public Simple_Collection<Drawable> {
public:
	Folder(std::string const& name_ = "folder");

	virtual void set_handle(entityHandle* handle_);
	
	void add(Drawable* elem);
};

#endif