
#ifndef FOLDER_HPP_
#define FOLDER_HPP_

#include "entity.hpp"

class folder : public entity {
public:
	folder(std::string const& name_ = "folder");
};

#endif