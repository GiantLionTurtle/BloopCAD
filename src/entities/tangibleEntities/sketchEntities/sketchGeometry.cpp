
#include "sketchGeometry.hpp"

sketchGeometry::sketchGeometry(geom_3d::plane_abstr_ptr basePlane_, int type_):
	sketchEntity(basePlane_, type_)
{
	
}

void sketchGeometry::for_each(std::function<void(entity_ptr geom)> func)
{
	for(constraintAnnotation_ptr annot : mAnnotations) {
		if(annot->exists())
			func(annot);
	}
}
void sketchGeometry::for_each(std::function<void(sketchEntity_ptr geom)> func)
{
	for(constraintAnnotation_ptr annot : mAnnotations) {
		if(annot->exists())
			func(annot);
	}
}
void sketchGeometry::for_each_annot(std::function<void(sketchEntity_ptr geom)> func)
{
	for(constraintAnnotation_ptr annot : mAnnotations) {
		if(annot->exists())
			func(annot);
	}
}

void sketchGeometry::add_constraintAnnotation(constraintAnnotation_ptr annot)
{
	if(annot) {
		mAnnotations.push_back(annot);
		annot->set_parent(this);
		if(selected()) {
			annot->set_hidden(false);
			set_require_redraw();
		} else {
			annot->set_hidden(true);
		}
		on_added_constraintAnnotation();
	}
}

void sketchGeometry::select_impl(bool sel)
{
	for(constraintAnnotation_ptr annot : mAnnotations) {
		annot->set_hidden(!sel);
	}	
}