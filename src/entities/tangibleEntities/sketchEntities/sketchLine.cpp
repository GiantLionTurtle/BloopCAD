
#include "sketchLine.hpp"

#include <utils/errorLogger.hpp>

sketchLine::sketchLine(line_abstract const& baseLine, bool immovable /*= false*/):
	line(baseLine)
{
	if(immovable) {
		mPointA->set_constant();
		mPointB->set_constant();
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
	return { {}, mPointA->pos_var()->expr() - mPointB->pos_var()->expr() };
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