
// BloopCAD
// Copyright (C) 2020-2021 BloopCorp

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "Constraint.hpp"

#include "Expression.hpp"
#include "ConstrSyst.hpp"
#include <Utils/Expunge.hpp>

#ifndef RELEASE_MODE
Constraint::Constraint(std::string name_, bool can_substitute)
	: mExists(true)
	, mCanSubstitute(can_substitute)
	, mName(name_)
{

}
#else
Constraint::Constraint(bool can_substitute)
	: mExists(true)
	, mCanSubstitute(can_substitute)
{

}
#endif
Constraint::~Constraint()
{

}

void Constraint::set_exists(bool ex)
{
	if(ex != exists()) {
		mExists = ex;
		if(mSyst)
			mSyst->force_decomposition();
		for(int i = 0; i < n_params(); ++i) {
			param(i)->set_exists(ex);
		}
		send(ex ? UI_Core_Link::LNK_ALIVE : UI_Core_Link::LNK_DEAD);
	}
}
void Constraint::receive(int msg, UI_Core_Link* sender)
{
	switch(msg) {
	case UI_Core_Link::LNK_ALIVE:
		set_exists(true);
		break;
	case UI_Core_Link::LNK_DEAD:
		set_exists(false);
		break;
	}
}

bool Constraint::satisfied()
{
	double err = std::abs(error());
	return (err < CONSTR_SATISFACTION_TRESHOLD);
}

void Constraint::add_blob(Param* a, Param* b, std::vector<SubstBlob*>& blobs)
{
	SubstBlob* blob_a = a->blob(), *blob_b = b->blob();

	if(blob_a == blob_b) {
		if(blob_a == nullptr) {
			blobs.push_back(new SubstBlob({a, b}));
		}
		return;
	}

	if(blob_a == nullptr && blob_b != nullptr) {
		blob_b->add_param(a);
		return;
	} else if(blob_a != nullptr && blob_b == nullptr) {
		blob_a->add_param(b);
		return;
	}

	blob_a->absorb(*blob_b);
	blobs.erase(std::remove(blobs.begin(), blobs.end(), blob_b), blobs.end());
	expunge(blob_b);
}

PointPoint_horizontality::PointPoint_horizontality(Geom2d::Point* p1, Geom2d::Point* p2)
#ifndef RELEASE_MODE
	: Constraint("PointPoint_horizontality", true)
#else
	: Constraint(true)
#endif
	, mP1(p1)
	, mP2(p2)
{

}

double PointPoint_horizontality::error()
{
	return mP1->y_val() - mP2->y_val();
}

Param* PointPoint_horizontality::param(int ind)
{
	switch(ind) {
	case 0: return mP1->y();
	case 1: return mP2->y();
	}
	return nullptr;
}
void PointPoint_horizontality::append_substBlobs(std::vector<SubstBlob*>& blobs)
{
	add_blob(mP1->y(), mP2->y(), blobs);
}


Line_horizontality::Line_horizontality(Geom2d::Line* l)
#ifndef RELEASE_MODE
	: Constraint("Line_horizontality", true)
#else
	: Constraint(true)
#endif
	, mLine(l)
{

}

double Line_horizontality::error()
{
	return mLine->ptA()->y()->val() - mLine->ptB()->y()->val();
}

Param* Line_horizontality::param(int ind)
{
	switch(ind) {
	case 0: return mLine->ptA()->y();
	case 1: return mLine->ptB()->y();
	}
	return nullptr;
}
void Line_horizontality::append_substBlobs(std::vector<SubstBlob*>& blobs)
{
	add_blob(mLine->ptA()->y(), mLine->ptB()->y(), blobs);
}


PointPoint_coincidence::PointPoint_coincidence(Geom2d::Point* p1, Geom2d::Point* p2)
#ifndef RELEASE_MODE
	: Constraint("PointPoint_coincidence", true)
#else
	: Constraint(true)
#endif
	, mP1(p1)
	, mP2(p2)
{

}

double PointPoint_coincidence::error()
{
	return (mP1->y()->val() - mP2->y()->val()) + (mP1->x()->val() - mP2->x()->val());
}

Param* PointPoint_coincidence::param(int ind)
{
	switch(ind) {
	case 0: return mP1->x();
	case 1: return mP1->y();
	case 2: return mP2->x();
	case 3: return mP2->y();
	}
	return nullptr;
}
void PointPoint_coincidence::append_substBlobs(std::vector<SubstBlob*>& blobs)
{
	add_blob(mP1->x(), mP2->x(), blobs);
	add_blob(mP1->y(), mP2->y(), blobs);
}


LineLine_angle::LineLine_angle(Geom2d::Line* l1, Geom2d::Line* l2, double ang)
#ifndef RELEASE_MODE
	: Constraint("LineLine_angle", false)
#else
	: Constraint(false)
#endif	
	, mL1(l1)
	, mL2(l2)
	, mAngle(ang)
{

}

double LineLine_angle::error()
{
	using std::pow;
	using std::sqrt;
	using std::abs;
	using std::fmod;
	using std::acos;

	// https://www.omnicalculator.com/math/angle-between-two-vectors

	// Describes the vectors of each line
	double x1 = mL1->ptA()->x()->val() - mL1->ptB()->x()->val();
	double y1 = mL1->ptA()->y()->val() - mL1->ptB()->y()->val();
	double x2 = mL2->ptA()->x()->val() - mL2->ptB()->x()->val();
	double y2 = mL2->ptA()->y()->val() - mL2->ptB()->y()->val();
 
	// All function take in double! If float functions are used it 
	// breaks everything because of precision issues
	return (fmod(acos((x1*x2+y1*y2) / (sqrt(x1*x1+y1*y1) * sqrt(x2*x2+y2*y2))), 2.0*M_PI) - mAngle);
}
double LineLine_angle::derivative(Param* withRespectTo)
{
	using std::pow;
	using std::sqrt;
	using std::abs;
	using std::fmod;
	using std::acos;

	// Put all the parameters values in variables to shorten code below
	// anyway it wouldn't be much clearer without it
	double a, b, c, d, e, f, g, h;
	a = param(0)->val();
	b = param(1)->val();

	c = param(2)->val();
	d = param(3)->val();

	e = param(4)->val();
	f = param(5)->val();

	g = param(6)->val();
	h = param(7)->val();


	// Compute each derivative with respect to the given parameter
	// they are probably correct
	// they were computed&simplified using 
	// https://www.derivative-calculator.net/ with a base formula of
	// acos(((a-c)*(e-g) + (b-d)*(f-h)) / (sqrt((a-c)*(a-c) + (b-d)*(b-d)) * sqrt((e-g)*(e-g) + (f-h)*(f-h)))) - pi/2
	// Note that the 'e' was replaced by another letter because 'e' is a constant
	
	// To know which derivative to hand in, the origins are compared
	// this is due to the fact that if some parameters are in a 
	// substitution blob, the blob will be passed through and not
	// the parameter.

	if(withRespectTo->origin() == param(0)->origin()) {

		return 	(d-b)*((h-f)*(a-c)+(d-b)*(g-e)) /
				((pow(a-c, 2.0)+pow(d-b, 2.0))*abs((h-f)*(a-c)+(d-b)*(g-e)));

	} else if(withRespectTo->origin() == param(1)->origin()) {

		return 	(c-a)*((g-e)*(b-d)+(c-a)*(h-f))	/
				((pow(b-d, 2.0)+pow(c-a, 2.0))*abs((g-e)*(b-d)+(c-a)*(h-f)));
	
	} else if(withRespectTo->origin() == param(2)->origin()) {

		return	(d-b)*((h-f)*(c-a)-(d-b)*(g-e)) / 
				((pow(c-a, 2.0)+pow(d-b, 2.0))*abs((h-f)*(c-a)-(d-b)*(g-e)));

	} else if(withRespectTo->origin() == param(3)->origin()) {

		return 	(c-a)*((g-e)*(d-b)-(c-a)*(h-f)) /
				((pow(d-b, 2.0)+pow(c-a, 2.0))*abs((g-e)*(d-b)-(c-a)*(h-f)));
	
	} else if(withRespectTo->origin() == param(4)->origin()) {

		return	(h-f)*((d-b)*(e-g)+(c-a)*(h-f)) /
				((pow(e-g, 2.0)+pow(h-f, 2.0))*abs((d-b)*(e-g)+(c-a)*(h-f)));
	
	} else if(withRespectTo->origin() == param(5)->origin()) {

		return	(g-e)*((c-a)*(f-h)+(d-b)*(g-e)) /
				((pow(f-h, 2.0)+pow(g-e, 2.0))*abs((c-a)*(f-h)+(d-b)*(g-e)));

	} else if(withRespectTo->origin() == param(6)->origin()) {

		double res = 	(h-f)*((d-b)*(g-e)-(c-a)*(h-f)) / 
						((pow(g-e, 2.0)+pow(h-f, 2.0))*abs((d-b)*(g-e)-(c-a)*(h-f)));
		return res;

	} else if(withRespectTo->origin() == param(7)->origin()) {

		return	(g-e)*((c-a)*(h-f)-(d-b)*(g-e)) /
				((pow(h-f, 2.0)+pow(g-e, 2.0))*abs((c-a)*(h-f)-(d-b)*(g-e)));

	}

	return 0.0; // This parameter is unknown, perhaps it is in another constraint
}
double LineLine_angle::stepScale(double lasterror)
{
	using std::abs;

	const double maxdErr = (10.0*M_PI/180.0); // max change in 10 degrees per iteration

	double scale = 1.0;
	double dError = 0.0;
	
	if((dError = abs(lasterror-error())) > maxdErr) {
		scale *= maxdErr / dError;
	}
	return scale;
}

void LineLine_angle::record_state()
{
	len1 = mL1->length();
	len2 = mL2->length();
}

Param* LineLine_angle::param(int ind)
{
	switch(ind) {
	case 0: return mL1->ptA()->x();
	case 1: return mL1->ptA()->y();
	case 2: return mL1->ptB()->x();
	case 3: return mL1->ptB()->y();
	case 4: return mL2->ptA()->x();
	case 5: return mL2->ptA()->y();
	case 6: return mL2->ptB()->x();
	case 7: return mL2->ptB()->y();
	}
	return nullptr;
}

Geom2d::Line* LineLine_angle::line(int ind)
{
	if(ind == 0) {
		return mL1;
	} else if(ind == 1) {
		return mL2;
	}
	return nullptr;
}

void LineLine_angle::set_angle(double ang)
{
	mAngle = ang;
}



test_towardZero_optim::test_towardZero_optim()
#ifndef RELEASE_MODE
	: Constraint("test_towardZero_optim", false)
#else
	: Constraint(false)
#endif
{

}

double test_towardZero_optim::error()
{
	return error_impl() * nerf;
}
double test_towardZero_optim::error_impl()
{
	std::cout<<"Bloop\n";
	using std::sqrt;
	using std::pow;

	int max_ind = std::max(mParams.size(), mInitVals.size());
	if(mParams.size() != mInitVals.size()) {
		std::cout<<"[e] Problem, param size = "<<mParams.size()<<",  initvals size = "<<mInitVals.size()<<"\n";
	}

	double inner_sum = 0.0;
	for(int i = 0; i < max_ind; ++i) {
		inner_sum += pow(mParams[i]->val() - mInitVals[i], 2.0);
		std::cout<<"sum: "<<inner_sum<<",  "<<mParams[i]->val()<<",  "<<mInitVals[i]<<"\n";
	}

	return sqrt(inner_sum);
}
double test_towardZero_optim::derivative(Param* withRespectTo)
{
	std::cout<<"Blip\n";
	double* target_orgigin = withRespectTo->origin();
	for(int i = 0; i < mParams.size(); ++i) {
		if(mParams[i]->origin() == target_orgigin) {
			double err = error_impl();
			if(err == 0.0)
				return 0.0;
			return ((mParams[i]->val() - mInitVals[i]) / err) * nerf;
		}
	}

	return 0.0;
}

void test_towardZero_optim::set_init(std::vector<double> ps)
{
	mInitVals = ps;
}

void test_towardZero_optim::set_params(std::vector<Param*> ps)
{
	mParams = ps;
}

Param* test_towardZero_optim::param(int ind)
{
	if(ind < 0 || ind >= mParams.size())
		return nullptr;

	return mParams[ind];
}


test_lineLength_optim::test_lineLength_optim()
#ifndef RELEASE_MODE
	: Constraint("test_lineLength_optim", false)
#else
	: Constraint(false)
#endif
{

}

double test_lineLength_optim::error()
{
	double sum = 0;

	for(int i = 0; i < mLines.size(); ++i) {
		sum += mLengths[i] - mLines[i]->length2();
	}
	return sum;
}
double test_lineLength_optim::derivative(Param* withRespectTo)
{
	double* target_orgigin = withRespectTo->origin();
	for(int i = 0; i < n_params(); ++i) {
		auto p = param(i);
		if(p->origin() == target_orgigin) {
			int paired_p = (i - i % 4) + (i % 4 + 2) % 4;
			return 2 * (p->val() - param(paired_p)->val());
		}
	}
	
	return 0.0;
}
void test_lineLength_optim::set_init()
{
	for(int i = 0; i < mLines.size(); ++i) {
		mLengths[i] = mLines[i]->length2();
	}
}
void test_lineLength_optim::set_lines(std::vector<Geom2d::Line*> lines)
{
	mLines = lines;
	mLengths.resize(lines.size());
}

Param* test_lineLength_optim::param(int ind)
{
	if(ind < 0 || ind >= mLines.size() * 4)
		return nullptr;
	
	Geom2d::Line* l = mLines[ind/4];
	int rem = ind % 4;

	switch(rem) {
	case 0:
		return l->ptA()->x();
	case 1:
		return l->ptA()->y();
	case 2: 
		return l->ptB()->x();
	case 3:
		return l->ptB()->y();
	}
	
	return nullptr;
}


// SkPointPoint_verticality::SkPointPoint_verticality(Geom3d::Plane_abstr* baseplane_, SkPoint* p1, SkPoint* p2):
// 	SkSprite_constraint<2>(baseplane_, { }, { p1->x() ^= p2->x() },
// 	"Resources/Textures/Images/Icons/Sketch/Constraints/Verticality.png"),
// 	mP1(p1),
// 	mP2(p2)
// {
// 	if(!mP1->fixed())
// 		mVars.push_back(mP1->x());
// 	if(!mP2->fixed())
// 		mVars.push_back(mP2->x());
// 	mP1->add_annot(annot(0));
// 	mP2->add_annot(annot(1));
// }

// SkLine_verticality::SkLine_verticality(Geom3d::Plane_abstr* baseplane_, SkLineCurve* l):
// 	SkSprite_constraint<1>(baseplane_, { }, { l->ptA()->x() ^= l->ptB()->x() },
// 	"Resources/Textures/Images/Icons/Sketch/Constraints/Verticality.png"),
// 	mLine(l)
// {
// 	if(!mLine->fixed())
// 		mVars.insert(mVars.end(), { l->ptA()->x(), l->ptB()->x() });
// 	mLine->add_annot(annot(0));
// }

// SkLine_length::SkLine_length(Geom3d::Plane_abstr* baseplane_, SkLineCurve* l, exp_ptr length):
// 	SkConstraint(baseplane_, { }, 
// 	{ pow(l->ptA()->x() - l->ptB()->x(), 2.0) + pow(l->ptA()->y() - l->ptB()->y(), 2.0) ^= pow(length, 2.0) }),
// 	mLine(l),
// 	mLength(length)
// {
// 	// Equation: pow(p1->x() - p2->x(), 2.0) + pow(p1->y() - p2->y(), 2.0) - pow(d, 2.0)

// 	// The square distance between two points is pow(p1->x() - p2->x(), 2.0) + pow(p1->y() - p2->y(), 2.0)
// 	// It must be equal to the square of d
// 	// TODO: check if squaring d truly is more efficient than taking the square root of the actual distance
// 	if(!mLine->fixed())
// 		mVars.insert(mVars.end(), { l->ptA()->x(), l->ptA()->y(), l->ptB()->x(), l->ptB()->y() });
// }

// SkPointPoint_horizontalDistance::SkPointPoint_horizontalDistance(Geom3d::Plane_abstr* baseplane_, 
// SkPoint* p1, SkPoint* p2, exp_ptr dist):
// 	SkConstraint(baseplane_, { }, { abs(p1->x() - p2->x()) ^= dist }),
// 	mP1(p1),
// 	mP2(p2),
// 	mDistance(dist)
// {
// 	if(!mP1->fixed())
// 		mVars.push_back(mP1->x());
// 	if(!mP2->fixed())
// 		mVars.push_back(mP2->x());
	
// }

// SkPointPoint_verticalDistance::SkPointPoint_verticalDistance(Geom3d::Plane_abstr* baseplane_, 
// SkPoint* p1, SkPoint* p2, exp_ptr dist):
// 	SkConstraint(baseplane_, { p1->y(), p2->y() }, { abs(p1->y() - p2->y()) ^= dist }),
// 	mP1(p1),
// 	mP2(p2),
// 	mDistance(dist)
// {
// 	if(!mP1->fixed())
// 		mVars.push_back(mP1->y());
// 	if(!mP2->fixed())
// 		mVars.push_back(mP2->y());	
// }

// // SkPointLine_distance_abstr::SkPointLine_distance_abstr(Geom3d::Plane_abstr* baseplane_, SkPoint* p, SkLineCurve* l, exp_ptr dist):
// // 	SkConstraint(baseplane_, { },
// // 	{ abs((l->ptB()->x()-l->ptA()->x())*(p->y() - l->ptA()->y()) - (l->ptB()->y()-l->ptA()->y())*(p->x() - l->ptA()->x())) / sqrt(l->length2()) ^= dist })
// // {
// // 	if(!p->fixed())
// // 		mVars.insert(mVars.end(), { p->x(), p->y() });
// // 	if(!l->fixed())
// // 		mVars.insert(mVars.end(), { l->ptA()->x(), l->ptA()->y(), l->ptB()->x(), l->ptB()->y() });
// // }


// // SkPointLine_distance::SkPointLine_distance(Geom3d::Plane_abstr* baseplane_, SkPoint* p, SkLineCurve* l, exp_ptr dist):
// // 	SkPointLine_distance_abstr(baseplane_, p, l, dist),
// // 	mPoint(p),
// // 	mLine(l),
// // 	mDistance(dist)
// // {

// // }



// // SkCircleLine_distance::SkCircleLine_distance(Geom3d::Plane_abstr* baseplane_, sketchCircle_ptr c, SkLineCurve* l, exp_ptr dist):
// // 	SkPointLine_distance_abstr(baseplane_, c->center(), l, dist + c->radius())
// // {

// // }

// SkLineLinePerpendicularity::SkLineLinePerpendicularity(Geom3d::Plane_abstr* baseplane_, SkLineCurve* l1, SkLineCurve* l2):
// 	SkSprite_constraint<2>(baseplane_, { }, { }, "Resources/Textures/Images/Icons/Sketch/Constraints/Perpendicularity.png"),
// 	mLine1(l1),
// 	mLine2(l2)
// {
// 	if(!l1->fixed()) {}
// 		mVars.insert(mVars.end(), { l1->ptA()->x(), l1->ptA()->y(), l1->ptB()->x(), l1->ptB()->y() });
// 	if(!l2->fixed())
// 		mVars.insert(mVars.end(), { l2->ptA()->x(), l2->ptA()->y(), l2->ptB()->x(), l2->ptB()->y() });

// 	exp_ptr x1 = (l1->ptA()->x()-l1->ptB()->x());
// 	exp_ptr y1 = (l1->ptA()->y()-l1->ptB()->y());
// 	exp_ptr x2 = (l2->ptA()->x()-l2->ptB()->x());
// 	exp_ptr y2 = (l2->ptA()->y()-l2->ptB()->y());

// 	mEqus.push_back(mod(acos(dot(x1, y1, x2, y2) / (sqrt(x1*x1+y1*y1) * sqrt(x2*x2+y2*y2))), 2.0*M_PI) ^= ExpConst::pi2);

// 	// https://www.omnicalculator.com/math/angle-between-two-vectors

// 	mLine1->add_annot(annot(0));
// 	mLine2->add_annot(annot(1));
// }

// SkPointPoint_coincidence::SkPointPoint_coincidence(Geom3d::Plane_abstr* baseplane_, SkPoint* p1, SkPoint* p2):
// 	SkSprite_constraint<1>(baseplane_, { }, { p1->x() ^= p2->x(), p1->y() ^= p2->y()}, 
// 	"Resources/Textures/Images/Icons/Sketch/Constraints/Coincidence.png"),
// 	mP1(p1),
// 	mP2(p2)
// {
// 	if(!mP1->fixed())
// 		mVars.insert(mVars.end(), { mP1->x(), mP1->y() });
// 	if(!mP2->fixed())
// 		mVars.insert(mVars.end(), { mP2->x(), mP2->y() });
	
// 	mP1->add_annot(annot(0));
// 	mP2->add_annot(annot(0));
// }
// SkPointLine_coincidence::SkPointLine_coincidence(Geom3d::Plane_abstr* baseplane_, SkPoint* p, SkLineCurve* l):
// 	SkSprite_constraint<1>(baseplane_, { },
// 	{ abs((l->ptB()->x()-l->ptA()->x())*(p->y() - l->ptA()->y()) - 
// 	(l->ptB()->y()-l->ptA()->y())*(p->x() - l->ptA()->x())) / sqrt(l->length2()) ^= ExpConst::zero }, 
// 	"Resources/Textures/Images/Icons/Sketch/Constraints/Coincidence.png"),	
// 	mPoint(p),
// 	mLine(l)
// {
// 	if(!p->fixed())
// 		mVars.insert(mVars.end(), { p->x(), p->y() });
// 	if(!l->fixed())
// 		mVars.insert(mVars.end(), { l->ptA()->x(), l->ptA()->y(), l->ptB()->x(), l->ptB()->y() });
	
// 	mPoint->add_annot(annot(0));
// }

// SkPointCircle_coincidence::SkPointCircle_coincidence(Geom3d::Plane_abstr* baseplane_, SkPoint* p, SkCircleCurve* c):
// 	SkSprite_constraint<1>(baseplane_, { },
// 	{ sqrt(pow(p->x() - c->center()->x(), 2) + pow(p->y() - c->center()->y(), 2)) ^= c->radius() }, 
// 	"Resources/Textures/Images/Icons/Sketch/Constraints/Coincidence.png"),	
// 	mPoint(p),
// 	mCircle(c)
// {
// 	if(!p->fixed())
// 		mVars.insert(mVars.end(), { p->x(), p->y() });
// 	if(!c->fixed())
// 		mVars.insert(mVars.end(), { c->center()->x(), c->center()->y(), c->radius() });
	
// 	mPoint->add_annot(annot(0));
// }