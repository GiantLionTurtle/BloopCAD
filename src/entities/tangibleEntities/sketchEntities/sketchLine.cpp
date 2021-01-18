
#include "sketchLine.hpp"

#include <utils/errorLogger.hpp>

expression_substr_funky::expression_substr_funky(expression_ptr left, expression_ptr right, double funkval):
	binary_expression(left, right),
	mFunk(false),
	mFunkval(funkval)
{
	mOp = operationType::SUBSTR;
}

double expression_substr_funky::eval()
{
	if(!mFunk)
		return mLeft->eval() - mRight->eval();
	return mFunkval;
}
expression_ptr expression_substr_funky::derivative()
{
	// std::cout<<"Der: "<<mLeft->derivative() - mRight->derivative()<<" => "<<(mLeft->derivative() - mRight->derivative())->eval()<<"\n";
	return mLeft->derivative() - mRight->derivative();
}

std::string expression_substr_funky::to_string()
{
	return "(" + mLeft->to_string() + " - " + mRight->to_string() + ")";
}


sketchLine::sketchLine(line_abstract const& baseLine, bool immovable /*= false*/):
	line(baseLine)
{
	if(immovable) {
		mPointA->set_constant();
		mPointB->set_constant();
	}

	dirX = std::make_shared<expression_substr_funky>(mPointA->pos_var()->expr()->x, mPointB->pos_var()->expr()->x, 0.0);
	dirY = std::make_shared<expression_substr_funky>(mPointA->pos_var()->expr()->y, mPointB->pos_var()->expr()->y, 0.0);
	dir = std::make_shared<expressionVector3>(dirX, dirY, std::make_shared<expression_const>(0.0));
}

void sketchLine::notify_childUpdate()
{
	tangibleEntity::notify_childUpdate();
	glm::vec2 dir = mPointA->pos_vec() - mPointB->pos_vec();
	if(glm::length2(dir) == 0.0f) {
		dirX->funk(true);
		dirY->funk(true);
	} else {
		dirX->funk(false);
		dirY->funk(false);
		dirX->set_funk(dir.x);
		dirY->set_funk(dir.y);
	}
}


std::vector<variable_ptr> sketchLine::variables()
{
	return { mPointA->pos_var()->x, mPointA->pos_var()->y, mPointB->pos_var()->x, mPointB->pos_var()->y };
}

subEquationsSystem sketchLine::coincidence()
{
	variable_ptr t = std::make_shared<variable>(0.5f);
	return { 	{ t }, 
				{ mPointA->pos_var()->x->expr() - t->expr() * (mPointA->pos_var()->x->expr() - mPointB->pos_var()->x->expr()),
				  mPointA->pos_var()->y->expr() - t->expr() * (mPointA->pos_var()->y->expr() - mPointB->pos_var()->y->expr()) }};
}
subEquationsSystem_vec sketchLine::direction()
{
	return { {}, dir }; // mPointA->pos_var()->expr() - mPointB->pos_var()->expr() };
}
std::vector<subEquationsSystem> sketchLine::verticality()
{
	return { { {}, { mPointA->pos_var()->x->expr() } },
			 { {}, { mPointB->pos_var()->x->expr() } }};
}
std::vector<subEquationsSystem> sketchLine::horizontality()
{
	return { { {}, { mPointA->pos_var()->y->expr() } },
			 { {}, { mPointB->pos_var()->y->expr() } }};
}