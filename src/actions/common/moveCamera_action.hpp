
#ifndef MOVECAMERA_ACTION_HPP_
#define MOVECAMERA_ACTION_HPP_

#include <actions/action.hpp>
#include <camera.hpp>

class document;

class moveCamera_action : public action {
private:
	document* mDoc;	// The target document
    camState mTarget;
public:
	moveCamera_action(document* doc, camState const& target, long duration_ms);

	virtual bool do_work();
	virtual bool undo_work();
};

#endif