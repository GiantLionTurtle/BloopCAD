
#include "Horizontality_tool.hpp"

#include <Drawables/Sk/SkLine.hpp>
#include <Workspaces/Workspace_abstr.hpp>
#include <Workspaces/Document.hpp>

Horizontality_tool::Horizontality_tool(Sketch_ws* env)
	: Constraint_tool(env)
{
	load_icon("Resources/Textures/Images/Icons/Sketch/Cursors/Horizontality_cursor.png");
}

int Horizontality_tool::could_add_geom(SkGeometry* geom)
{
	if(!geom) {
		return add_states::COULDNT_ADD;
	} else if(mEntA) {
		if(is_point(geom))
			return add_states::WOULD_BE_COMPLETE;
	} else {
		if(is_point(geom)) {
			return add_states::COULD_ADD;
		} else if(is_line(geom)) {
			return add_states::WOULD_BE_COMPLETE;
		}
	}
	return add_states::COULDNT_ADD;
}

void Horizontality_tool::create_constraint(Constraint*& constr, SkGeometry*& priority_ent, Action_ptr& annotAct)
{
	if(!mEntA && !mEntB) {
		LOG_WARNING("Attempting to add incomplete constraint.");
		return;
	}

	if(!mEntB) {
		constr = new Line_horizontality(static_cast<SkLine*>(mEntA)->geom());

		SkConstrAnnot* annot = nullptr;
		SkConstrAnnot::make_single_annot(annot, constr, mEnv->target()->basePlane(), 
		"Resources/Textures/Images/Icons/Sketch/Constraints/Horizontality.png");
		mEnv->target()->add_annot(annot);
		mEntA->add_annot(annot);
	} else {
		constr = new PointPoint_horizontality(static_cast<SkPoint*>(mEntA)->geom(), static_cast<SkPoint*>(mEntB)->geom());
		priority_ent = mEntA;

		SkConstrAnnot* annot_a = nullptr, *annot_b = nullptr;
		SkConstrAnnot::make_dual_annot(annot_a, annot_b, constr, mEnv->target()->basePlane(), 
		"Resources/Textures/Images/Icons/Sketch/Constraints/Horizontality.png");
		mEnv->target()->add_annot(annot_a);
		mEnv->target()->add_annot(annot_b);
		mEntA->add_annot(annot_a);
		mEntB->add_annot(annot_b);
	}
}