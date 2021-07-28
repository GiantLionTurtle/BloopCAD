
#ifndef SKETCHPOINT_HPP_
#define SKETCHPOINT_HPP_

#include "sketchGeometry.hpp"
#include "entityPosSnapshot_abstract.hpp"
#include <geometry/geometry_2d/Point_abstr.hpp>
#include <constraintsSolver/expression.hpp>

class sketchPoint;
using sketchPoint_ptr = std::shared_ptr<sketchPoint>;

class sketchPoint : public sketchGeometry, public Geom2d::Point_abstr, public std::enable_shared_from_this<sketchPoint> {
private:
	static float kSelDist2;
	static bool kFisrstInst;
	static glm::vec3 kColor, kColorHovered, kColorSelected; // Point color

	var_ptr mX, mY;
public:
	sketchPoint(glm::vec2 pos_2d, Geom3d::plane_abstr* basePlane_, bool immovable = false);
	sketchPoint(var_ptr x_, var_ptr y_, Geom3d::plane_abstr* basePlane_, bool immovable = false);
	virtual ~sketchPoint() {}

	void print(int depth = 0);

	void init();

	glm::vec2 pos() const { return glm::vec2(mX->eval(), mY->eval()); }
	var_ptr x() { return mX; }
	var_ptr y() { return mY; }

	void move(glm::vec2 from, glm::vec2 to, glm::vec2 pixel_move);

	bool in_selection_range(glm::vec2 planepos, Camera_ptr cam, glm::vec2 cursor);
	bool in_selection_range(glm::vec2 a, glm::vec2 b, bool contained);

	void for_each(std::function<void(sketchGeometry_ptr geom)> func) {}

	void set(glm::vec2 p);

	virtual entityPosSnapshot_ptr posSnapshot();

	void set_constant();
	void set_tmpConstant(bool const_);

	int selection_rank() { return 5; }

	void update_VB();

	glm::vec2 annotation_pixelOffset(int ind);
	void position_floatingAnnotation(std::shared_ptr<spriteAnnotation> annot);

	void set_exists_vars(bool ex);

	void print_ids() { std::cout<<"Point: "<<mX->id()<<",  "<<mY->id()<<"\n"; }
protected:
	void draw_impl(Camera_ptr cam, int frame);

	void post_set_behavior();

	void set_dragged_impl(bool drag);
};

class pointSnapshot : public entityPosSnapshot_abstract {
private:
	sketchPoint_ptr mPoint;
	double x, y;
public:
	pointSnapshot(sketchPoint_ptr pt);
	pointSnapshot(sketchPoint_ptr source, sketchPoint_ptr target);

	sketchEntity_ptr ent() { return mPoint; }
	void apply();
};


#endif 