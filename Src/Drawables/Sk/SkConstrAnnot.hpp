
#ifndef SKCONSTRANNOT_HPP_
#define SKCONSTRANNOT_HPP_

#include <Drawables/Base/Drawable.hpp>
#include <Drawables/Base/Collection.hpp>
#include "SkSprite.hpp"

class SkConstrAnnot : public SkSprite {

};

class SkConstrTwinAnnot : public LinearFixed_indexer<SkConstrAnnot, 2> {

};

#endif