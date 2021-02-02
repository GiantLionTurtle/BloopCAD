
#ifndef SKETCHLINE_HPP_
#define SKETCHLINE_HPP_

#include "sketchEntity.hpp"
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


class sketchLine : public sketchEntity, public geom_2d::line_abstr {
private:
	std::shared_ptr<expression_substr_funky> dirX, dirY;
	expressionVector2_ptr dir;

	const glm::vec3 mColor = glm::vec3(0.0f, 0.89f, 0.725f); // Line color
	glm::vec3 mVertices[2]; // The vertices describing the line
public:
	sketchLine(line_abstr const& baseLine, geom_3d::plane_abstr_ptr basePlane_, bool immovable = false);
	virtual ~sketchLine() {}

	virtual void init();

	virtual int selection_rank() { return 7; }

    virtual void notify_childUpdate();

	virtual std::vector<variable_ptr> variables();

	virtual subEquationsSystem coincidence();
	virtual subEquationsSystem_vec direction();
	virtual std::vector<subEquationsSystem> verticality();
	virtual std::vector<subEquationsSystem> horizontality();
	virtual void update_VB();
protected:
	/*
		@function draw_impl draws the line on screen

		@param cam : 	The camera used for rendering
		@param frame : 	The current frame id
	*/
	virtual void draw_impl(camera_ptr cam, int frame);

	virtual float selection_depth(camera_ptr cam, glm::vec2 cursor_pos);

	virtual void post_set_behavior() {}
};

#endif 