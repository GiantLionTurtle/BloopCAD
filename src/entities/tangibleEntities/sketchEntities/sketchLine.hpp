
#ifndef SKETCHLINE_HPP_
#define SKETCHLINE_HPP_

#include "sketchElement.hpp"
#include <entities/tangibleEntities/line.hpp>

class sketchLine;
using sketchLine_ptr = std::shared_ptr<sketchLine>;

class sketchLine : public sketchElement, public line {
public:
	sketchLine(line_abstract const& baseLine, bool immovable = false);

    virtual void set_basePlane(plane_abstract_ptr basePlane_) { mBasePlane = basePlane_; mRequire_VBUpdate = true; }

	virtual subEquationsSystem coincidence();
};

#endif 