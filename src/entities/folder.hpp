
#ifndef FOLDER_HPP_
#define FOLDER_HPP_

#include "entity.hpp"

class folder;
using folder_ptr = std::shared_ptr<folder>;

class folder : public entity {
protected:
	std::vector<entity_ptr> mChildren;
public:
	folder(std::string const& name_ = "folder", entity* parent_ = nullptr);

	virtual void set_handle(entityHandle* handle_);
	
	void add(entity_ptr elem);

	virtual void for_each(std::function<void (entity_ptr)> func);
};

#endif