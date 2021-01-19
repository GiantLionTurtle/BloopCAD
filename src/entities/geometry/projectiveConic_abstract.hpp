
// #ifndef CONIC_ABSTRACT_HPP_
// #define CONIC_ABSTRACT_HPP_

// #include "point_abstract.hpp"

// #include <glm/glm.hpp>
// #include <iostream>

// // http://jwilson.coe.uga.edu/EMAT6680Fa08/Kuzle/Essay%201/Conics%20in%20Projective%20Geometry.html
// // https://math.stackexchange.com/questions/2388747/formula-for-ellipse-formed-from-projecting-a-tilted-circle-onto-the-xy-plane
// // https://math.stackexchange.com/questions/1367710/perspective-projection-of-a-sphere-on-a-plane

// class projectiveConic_abstract;
// using projectiveConic_abstract_ptr = std::shared_ptr<projectiveConic_abstract>;

// class projectiveConic_abstract {
// protected:
// 	float e;
// public:
// 	// circle_abstract(point_abstract centerPoint, variable_ptr radius_);
// 	// circle_abstract(point_abstract_ptr ptA, point_abstract_ptr ptB);
// 	// virtual ~circle_abstract() {};

// 	// point_abstract_ptr center() const { return mCenterPoint; }
// 	// void set_center(point_abstract_ptr pt) { mCenterPoint->set(pt); }
//     // variable_ptr radius() const { return mRadius; }

// 	// float dist_to_point(point_abstract const& pt);
// 	// point_abstract_ptr closest_point(point_abstract const& pt);
// 	// point_abstract_ptr closest_point(point_abstract const& pt, float& at);

// 	// point_abstract_ptr at(float at);

// 	bool is_circle() const 		{ return e == 0; }
// 	bool is_elipse() const 		{ return e > 0.0f && e < 1.0f; }
// 	bool is_parabola() const 	{ return e == 1.0f; }
// 	bool is_hyperbola() const 	{ return e > 1.0f; }
// };

// #endif

// // 

