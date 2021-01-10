
#ifndef FOLDER_HPP_
#define FOLDER_HPP_

#include "entity.hpp"

class folder;
using folder_ptr = std::shared_ptr<folder>;

class folder : public entity {
public:
	folder(std::string const& name_ = "folder");

	virtual void set_handle(entityHandle* handle_);
	
	virtual void add(entity_ptr elem);
};

#endif