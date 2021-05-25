
#ifndef SKETCHGEOMETRY_HPP_
#define SKETCHGEOMETRY_HPP_

#include "sketchEntity.hpp"
#include "constraintAnnotation.hpp"

class sketchGeometry;
using sketchGeometry_ptr = std::shared_ptr<sketchGeometry>;

class sketchGeometry : public sketchEntity {
protected:
	std::vector<constraintAnnotation_ptr> mAnnotations;
public:
	sketchGeometry(geom_3d::plane_abstr_ptr basePlane_, int type_);

	virtual void for_each(std::function<void(entity_ptr geom)> func);
	virtual void for_each(std::function<void(sketchEntity_ptr geom)> func);
	virtual void for_each(std::function<void(sketchGeometry_ptr geom)> func) = 0;
	void for_each_annot(std::function<void(sketchEntity_ptr geom)> func);

	void add_constraintAnnotation(constraintAnnotation_ptr annot);
	virtual void on_added_constraintAnnotation() {}

	void select_impl(bool sel);
};


#endif