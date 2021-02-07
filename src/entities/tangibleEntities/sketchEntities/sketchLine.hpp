
#ifndef SKETCHLINE_HPP_
#define SKETCHLINE_HPP_

#include "sketchEntity.hpp"
#include "sketchPoint.hpp"
#include <geometry/geometry_2d/line_abstr.hpp>
#include <entities/sketch.hpp>

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


class sketchLine : public sketchEntity, public geom_2d::line_abstr {
private:
	std::shared_ptr<expression_substr_funky> dirX, dirY;
	expressionVector2_ptr dir;

	const glm::vec3 mColor = glm::vec3(0.0f, 0.89f, 0.725f); // Line color
	glm::vec3 mVertices[2]; // The vertices describing the line

	sketchPoint_ptr mA, mB;
public:
	sketchLine(sketchPoint_ptr ptA, sketchPoint_ptr ptB, sketch* parent_sk, bool immovable = false);
	sketchLine(glm::vec2 ptA, glm::vec2 ptB, sketch* parent_sk, bool immovable = false);
	virtual ~sketchLine() {}

	void notify(int msg = 0) { set_require_VBUpdate(); }

	void init();

	void for_each(std::function<void (entity_ptr)> func);

	void move(glm::vec2 from, glm::vec2 to);

	bool in_selection_range(glm::vec2 planepos, camera_ptr cam, glm::vec2 cursor);

	void notify_childUpdate();

	glm::vec2 A() { return mA->pos(); }
	glm::vec2 B() { return mB->pos(); }


	int selection_rank() { return 3; }

	std::vector<variable_ptr> variables();

	subEquationsSystem coincidence();
	subEquationsSystem_vec direction();
	std::vector<subEquationsSystem> verticality();
	std::vector<subEquationsSystem> horizontality();
	void update_VB();
protected:
	/*
		@function draw_impl draws the line on screen

		@param cam : 	The camera used for rendering
		@param frame : 	The current frame id
	*/
	void draw_impl(camera_ptr cam, int frame);

	// float selection_depth(camera_ptr cam, glm::vec2 cursor_pos);
};

#endif 