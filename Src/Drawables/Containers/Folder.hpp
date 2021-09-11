
#ifndef FOLDER_HPP_
#define FOLDER_HPP_

#include <Drawables/Base/Collection.hpp>

class Folder;
using Folder_ptr = std::shared_ptr<Folder>;

class Folder : public Collection_abstr<LinearResizable_indexer<Drawable*>> {
public:
	Folder(std::string const& name_ = "folder");

	virtual void set_handle(UILink* handle_);
	
	void add(Drawable* elem);
	Drawable* get(size_t ind);
};

#endif