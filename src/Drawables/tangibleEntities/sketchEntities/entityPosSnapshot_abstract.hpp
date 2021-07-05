
#ifndef ENTITYPOSSNAPSHOT_ABSTRACT_HPP_
#define ENTITYPOSSNAPSHOT_ABSTRACT_HPP_

#include "sketchEntity.hpp"

class entityPosSnapshot_abstract {
public:
	~entityPosSnapshot_abstract() {}

	virtual sketchEntity_ptr ent() = 0;
	virtual void apply() = 0;
};

#endif