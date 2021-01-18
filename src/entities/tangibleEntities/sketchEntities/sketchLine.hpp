
#ifndef SKETCHLINE_HPP_
#define SKETCHLINE_HPP_

#include "sketchElement.hpp"
#include <entities/tangibleEntities/line.hpp>

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


class sketchLine : public sketchElement, public line {
private:
	std::shared_ptr<expression_substr_funky> dirX, dirY;
	expressionVector3_ptr dir;
public:
	sketchLine(line_abstract const& baseLine, bool immovable = false);

    virtual void set_basePlane(plane_abstract_ptr basePlane_) { mBasePlane = basePlane_; mRequire_VBUpdate = true; }

    virtual void notify_childUpdate();

	virtual std::vector<variable_ptr> variables();

	virtual subEquationsSystem coincidence();
	virtual subEquationsSystem_vec direction();
	virtual std::vector<subEquationsSystem> verticality();
	virtual std::vector<subEquationsSystem> horizontality();
};

#endif 