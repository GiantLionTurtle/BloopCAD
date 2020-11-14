
#ifndef CREATESKETCH_ACTION_HPP_
#define CREATESKETCH_ACRION_HPP_

#include <actions/action.hpp>
#include <entities/plane_abstract.hpp>
#include <entities/part.hpp>
#include <entities/sketch.hpp>

#include <memory>

class createSketch_action : public action {
private:
	std::shared_ptr<plane_abstract> mSketchPlane;
	std::shared_ptr<part> mTarget;
	std::shared_ptr<sketch> mSketch;
public:
	createSketch_action(std::shared_ptr<plane_abstract> sketchPlane, std::shared_ptr<part> target);

	virtual void do_work();
	virtual void undo_work();
};

#endif