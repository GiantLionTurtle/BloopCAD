
#ifndef EXPRESSIONVEC_HPP_
#define EXPRESSIONVEC_HPP_

#include "Expression.hpp"

#include <Geom/Geom2d/Point_abstr.hpp>

#include <array>
#include <memory>

template<typename eT, size_t nD>
class ExpVec {
protected:
	std::array<std::shared_ptr<eT>, nD> mComponents;
public:
	ExpVec(std::array<std::shared_ptr<eT>, nD> comps):
		mComponents(comps)
	{

	}
	ExpVec():
		mComponents({})
	{

	}

	inline constexpr size_t dims() { return nD; }
	inline std::shared_ptr<eT>& comp(size_t ind)
	{
		return mComponents.at(ind);
	}
};

template<typename eT>
class ExpVec2 : public ExpVec<eT, 2>, public Geom2d::Point_abstr{
public:
	ExpVec2(std::shared_ptr<eT> x_, std::shared_ptr<eT> y_):
		ExpVec<eT, 2>({ x_, y_ })
	{

	}
	ExpVec2()
	{

	}
	inline std::shared_ptr<eT>& x() { return ExpVec<eT, 2>::comp(0); }
	inline std::shared_ptr<eT>& y() { return ExpVec<eT, 2>::comp(1); }
	glm::vec2 pos() { return glm::vec2(x()->eval(), y()->eval()); }
	void set(glm::vec2 p) {}
};

#define ELEMENTWISE_FUNCTOR_SINGLE(vecType, func) \
template<typename eTi> \
vecType<Expression> func(vecType<eTi> expVec) \
{ \
	vecType<Expression> newExpVec; \
	for(size_t i = 0; i < expVec.dims(); ++i) { \
		newExpVec.comp(i) = func(expVec.comp(i)); \
	} \
	return newExpVec; \
}

#define ELEMENTWISE_FUNCTOR_EXP_EXP(vecType, func) \
template<typename eTi1, typename eTi2> \
vecType<Expression> func(vecType<eTi1> expVec1, vecType<eTi2> expVec2) \
{ \
	vecType<Expression> newExpVec; \
	for(size_t i = 0; i < expVec1.dims(); ++i) { \
		newExpVec.comp(i) = func(expVec1.comp(i), expVec2.comp(i)); \
	} \
	return newExpVec; \
}

#define ELEMENTWISE_FUNCTOR_DBL_EXP(vecType, func) \
template<typename eTi> \
vecType<Expression> func(double val, vecType<eTi> expVec1) \
{ \
	vecType<Expression> newExpVec; \
	for(size_t i = 0; i < expVec1.dims(); ++i) { \
		newExpVec.comp(i) = func(val, expVec1.comp(i)); \
	} \
	return newExpVec; \
}

#define ELEMENTWISE_FUNCTOR_EXP_DBL(vecType, func) \
template<typename eTi1> \
vecType<Expression> func(vecType<eTi1> expVec1, double val) \
{ \
	vecType<Expression> newExpVec; \
	for(size_t i = 0; i < expVec1.dims(); ++i) { \
		newExpVec.comp(i) = func(expVec1.comp(i), val); \
	} \
	return newExpVec; \
}



ELEMENTWISE_FUNCTOR_EXP_EXP(ExpVec2, operator+);
ELEMENTWISE_FUNCTOR_EXP_EXP(ExpVec2, operator-);
ELEMENTWISE_FUNCTOR_EXP_EXP(ExpVec2, operator*);
ELEMENTWISE_FUNCTOR_EXP_EXP(ExpVec2, operator/);
ELEMENTWISE_FUNCTOR_DBL_EXP(ExpVec2, operator+);
ELEMENTWISE_FUNCTOR_DBL_EXP(ExpVec2, operator-);
ELEMENTWISE_FUNCTOR_DBL_EXP(ExpVec2, operator*);
ELEMENTWISE_FUNCTOR_DBL_EXP(ExpVec2, operator/);
ELEMENTWISE_FUNCTOR_EXP_DBL(ExpVec2, operator+);
ELEMENTWISE_FUNCTOR_EXP_DBL(ExpVec2, operator-);
ELEMENTWISE_FUNCTOR_EXP_DBL(ExpVec2, operator*);
ELEMENTWISE_FUNCTOR_EXP_DBL(ExpVec2, operator/);
ELEMENTWISE_FUNCTOR_SINGLE(ExpVec2, operator+);
ELEMENTWISE_FUNCTOR_SINGLE(ExpVec2, operator-);

ELEMENTWISE_FUNCTOR_EXP_DBL(ExpVec2, pow);
ELEMENTWISE_FUNCTOR_EXP_DBL(ExpVec2, mod);

template<typename eT>
exp_ptr dot(ExpVec2<eT> a, ExpVec2<eT> b)
{
	return dot(a.x(), a.y(), b.x(), b.y());
}

ELEMENTWISE_FUNCTOR_EXP_EXP(ExpVec2, pow);
ELEMENTWISE_FUNCTOR_SINGLE(ExpVec2, sqrt);
ELEMENTWISE_FUNCTOR_SINGLE(ExpVec2, sin)
ELEMENTWISE_FUNCTOR_SINGLE(ExpVec2, asin);
ELEMENTWISE_FUNCTOR_SINGLE(ExpVec2, csc);
ELEMENTWISE_FUNCTOR_SINGLE(ExpVec2, cos);
ELEMENTWISE_FUNCTOR_SINGLE(ExpVec2, acos);
ELEMENTWISE_FUNCTOR_SINGLE(ExpVec2, sec);
ELEMENTWISE_FUNCTOR_SINGLE(ExpVec2, tan);
ELEMENTWISE_FUNCTOR_SINGLE(ExpVec2, atan2);
ELEMENTWISE_FUNCTOR_SINGLE(ExpVec2, cot);
ELEMENTWISE_FUNCTOR_SINGLE(ExpVec2, abs);

#endif