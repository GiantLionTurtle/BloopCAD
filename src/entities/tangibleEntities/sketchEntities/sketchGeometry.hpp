
#ifndef SKETCHGEOMETRY_HPP_
#define SKETCHGEOMETRY_HPP_

#include "sketchEntity.hpp"
#include "constraintAnnotation.hpp"
// #include <constraintsSolver/constraint.hpp>

class sketchGeometry;
using sketchGeometry_ptr = std::shared_ptr<sketchGeometry>;

class sketchGeometry : public sketchEntity {
protected:
	std::vector<std::shared_ptr<constraintAnnotation>> mAnnotations;
public:
	sketchGeometry(geom_3d::plane_abstr_ptr basePlane_, int type_):
		sketchEntity(basePlane_, type_)
	{}

	virtual void for_each(std::function<void(entity_ptr geom)> func)
	{
		for(std::shared_ptr<constraintAnnotation> annot : mAnnotations) {
			func(annot);
		}
	}
	virtual void for_each(std::function<void(sketchEntity_ptr geom)> func)
	{
		for(std::shared_ptr<constraintAnnotation> annot : mAnnotations) {
			func(annot);
		}
	}

	void add_constraintAnnotation(std::shared_ptr<constraintAnnotation> annot)
	{
		if(annot) {
			mAnnotations.push_back(annot);
			if(selected()) {
				annot->set_hidden(false);
			} else {
				annot->set_hidden(true);
			}
			on_added_constraintAnnotation();
		}
	}
	virtual void on_added_constraintAnnotation() {}

	void select_impl(bool sel)
	{
		for(std::shared_ptr<constraintAnnotation> annot : mAnnotations) {
			annot->set_hidden(!sel);
		}
	}
};


#endif