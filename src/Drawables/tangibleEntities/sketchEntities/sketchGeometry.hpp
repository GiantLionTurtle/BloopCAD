
#ifndef SKETCHGEOMETRY_HPP_
#define SKETCHGEOMETRY_HPP_

#include "sketchEntity.hpp"
#include "constraintAnnotation.hpp"
#include "sketchConstraint.hpp"

class sketchGeometry;
using sketchGeometry_ptr = std::shared_ptr<sketchGeometry>;

class sketchGeometry : public sketchEntity {
protected:
	// std::vector<constraintAnnotation_ptr> mAnnotations; // ejnkmwlf
	std::vector<sketchEntity_ptr> mMiscannots;
	std::vector<std::shared_ptr<spriteAnnotation>> mFloatingAnnots;
	int mGeomID;
	static int numGeoms;
public:
	sketchGeometry(Geom3d::plane_abstr* basePlane_, int type_);

	virtual void for_each(std::function<void(Drawable* geom)> func);
	virtual void for_each(std::function<void(sketchEntity_ptr geom)> func);
	virtual void for_each(std::function<void(sketchGeometry_ptr geom)> func) = 0;
	void for_each_annot(std::function<void(sketchEntity_ptr geom)> func);

	virtual bool fixed() { return false; } // Not really developped, will be usefull for projected geometries and axis and whatnot

	// void add_constraintAnnotation(constraintAnnotation_ptr annot);
	// virtual void on_added_constraintAnnotation() {}

	void add_annotation(sketchEntity_ptr annot);
	void add_floatingAnnotation(std::shared_ptr<spriteAnnotation> annot);
	virtual void position_floatingAnnotation(std::shared_ptr<spriteAnnotation> annot) { }

	void select_impl(bool sel);
};


#endif