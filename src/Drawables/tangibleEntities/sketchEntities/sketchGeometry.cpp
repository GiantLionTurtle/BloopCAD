
#include "sketchGeometry.hpp"

int sketchGeometry::numGeoms = 0;

sketchGeometry::sketchGeometry(geom_3d::plane_abstr_ptr basePlane_, int type_):
	sketchEntity(basePlane_, type_),
	mGeomID(numGeoms++)
{
	
}

void sketchGeometry::for_each(std::function<void(Drawable* geom)> func)
{
	// for(constraintAnnotation_ptr annot : mAnnotations) {
	// 	if(annot->exists())
	// 		func(annot);
	// }
}
void sketchGeometry::for_each(std::function<void(sketchEntity_ptr geom)> func)
{
	// for(constraintAnnotation_ptr annot : mAnnotations) {
	// 	if(annot->exists())
	// 		func(annot);
	// }
}
void sketchGeometry::for_each_annot(std::function<void(sketchEntity_ptr geom)> func)
{
	// for(constraintAnnotation_ptr annot : mAnnotations) {
	// 	if(annot->exists())
	// 		func(annot);
	// }
}

// void sketchGeometry::add_constraintAnnotation(constraintAnnotation_ptr annot)
// {
// 	// &^& Delete
// 	if(annot) {
// 		mAnnotations.push_back(annot);
// 		annot->set_parent(this);
// 		if(selected()) {
// 			annot->set_hidden(false);
// 			set_need_redraw();
// 		} else {
// 			annot->set_hidden(true);
// 		}
// 		on_added_constraintAnnotation();
// 	}
// }


void sketchGeometry::add_annotation(sketchEntity_ptr annot)
{
	mMiscannots.push_back(annot);
}
void sketchGeometry::add_floatingAnnotation(std::shared_ptr<spriteAnnotation> annot)
{
	mFloatingAnnots.push_back(annot);
	position_floatingAnnotation(annot);
}
void sketchGeometry::select_impl(bool sel)
{
	// // &^& Cleanup
	// for(constraintAnnotation_ptr annot : mAnnotations) {
	// 	annot->set_hidden(!sel);
	// }

	for(auto annot : mFloatingAnnots) {
		annot->set_hidden(!sel);
	}
}