
#ifndef SKETCHLINE_HPP_
#define SKETCHLINE_HPP_

#include "sketchGeometry.hpp"
#include "sketchPoint.hpp"
#include <geometry/geometry_2d/line_abstr.hpp>

class sketchLine;
using sketchLine_ptr = std::shared_ptr<sketchLine>;

class expression_substr_funky : public binary_expression {
private:
	bool mFunk;
	double mFunkval;
public:
	expression_substr_funky(expression_ptr left, expression_ptr right, double funkval);

	virtual double eval();
	virtual expression_ptr derivative();

	virtual std::string to_string();

	void funk(bool do_)
	{
		mFunk = do_;
	}
	void set_funk(double funkval)
	{
		mFunkval = funkval;
	}
};

class sketchLine : public sketchGeometry, public geom_2d::line_abstr {
private:
	// std::shared_ptr<expression_substr_funky> dirX, dirY;
	// expressionVector2_ptr dir;

	static float kSelDist2;
	static bool kFisrstInst;
	static glm::vec3 kColor, kColorHovered, kColorSelected; // Line color

	glm::vec3 mVertices[2]; // The vertices describing the line

	sketchPoint_ptr mA, mB;
	expression_ptr mLength2;
public:
	sketchLine(sketchPoint_ptr ptA, sketchPoint_ptr ptB, geom_3d::plane_abstr_ptr basePlane_, bool immovable = false);
	sketchLine(glm::vec2 ptA, glm::vec2 ptB, geom_3d::plane_abstr_ptr basePlane_, bool immovable = false);
	virtual ~sketchLine() {}

	void notify(int msg = 0) { set_require_VBUpdate(); }

	void init();

	virtual void print(int depth = 0);

	void for_each(std::function<void (entity_ptr)> func);
	void for_each(std::function<void(sketchEntity_ptr ent)> func);
	
	void move(glm::vec2 from, glm::vec2 to);

	bool in_selection_range(glm::vec2 planepos, camera_ptr cam, glm::vec2 cursor);
	bool in_selection_range(glm::vec2 a, glm::vec2 b, bool contained);

	void set_constant();
	void set_tmpConstant(bool const_);

	glm::vec2 posA() { return mA->pos(); }
	glm::vec2 posB() { return mB->pos(); }
	sketchPoint_ptr A() { return mA; }
	sketchPoint_ptr B() { return mB; }

	sketchLine_ptr clone();

	expression_ptr length2() { return mLength2; }

	int selection_rank() { return 3; }

	void update_VB();

	void on_added_constraintAnnotation();

	glm::vec2 annotation_pixelOffset(int ind);

	void set_exists_vars(bool ex);
protected:
	/*
		@function draw_impl draws the line on screen

		@param cam : 	The camera used for rendering
		@param frame : 	The current frame id
	*/
	void draw_impl(camera_ptr cam, int frame);
};

#endif 