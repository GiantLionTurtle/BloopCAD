
// BloopCAD
// Copyright (C) 2020  BloopCorp

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

#ifndef PLANE_ABSTR3_HPP_
#define PLANE_ABSTR3_HPP_

#include "Geom3d_abstr.hpp"
#include "Point_abstr.hpp"
#include <Geometry/2d/Point_abstr.hpp>

#include <glm/glm.hpp>

#include <memory>

namespace Geom3d {

/*
	@class Plane_abstract describes an abstract representation of a plane
*/
class Plane_abstr {
protected:
	glm::vec3 mOrigin, mV, mW; // The origin point of the plane and two vectors that are on the plane and non-parallel
	float mDir; // Multiplicative factor to inverse a plane
public:
	/*
		@function Plane_abstract creates a plane abstract

		@param origin :							The origin of the plane
		@param v : 								One of the two vectors that lie on the plane
		@param w : 								The other vector that lies on the plane
		@param reversed [defaults to false] : 	Wheter or not the direction should be inverted
	*/
	Plane_abstr(glm::vec3 const& origin, glm::vec3 const& v, glm::vec3 const& w, bool reversed = false);
	virtual ~Plane_abstr();
	
	// static Plane_abstract from_1Point1Vector(glm::vec3 origin, glm::vec3 n);
	/*
		@function from_1Point2Vectors creates a Plane_abstract

		@param origin :							The origin of the plane
		@param v : 								One of the two vectors that lie on the plane
		@param w : 								The other vector that lies on the plane
		@param reversed [defaults to false] : 	Wheter or not the direction should be inverted

		@return : The created plane
	*/
	static Plane_abstr from_1Point2Vectors(glm::vec3 const& origin, glm::vec3 const& v, glm::vec3 const& w, bool reversed = false); // this one is just a wrapper around the basic constructor
	// static Plane_abstract from_3Points(glm::vec3 origin, glm::vec3 pt1, glm::vec3 pt2);

	// static Plane_abstract_ptr from_1Point1Vector_ptr(glm::vec3 origin, glm::vec3 n);
	/*
		@function from_1Point2Vectors_ptr creates a Plane_abstract

		@param origin :							The origin of the plane
		@param v : 								One of the two vectors that lie on the plane
		@param w : 								The other vector that lies on the plane
		@param reversed [defaults to false] : 	Wheter or not the direction should be inverted

		@return : A pointer to the created plane
	*/
	static Plane_abstr* from_1Point2Vectors_ptr(glm::vec3 const& origin, glm::vec3 const& v, glm::vec3 const& w, bool reversed = false); // this one is just a wrapper around the basic constructor
	// static Plane_abstract_ptr from_3Points_ptr(glm::vec3 origin, glm::vec3 pt1, glm::vec3 pt2);

	/*
		@function dist_to_point gives the distance between a point and the plane

		@param point : The point from which to calculate the distance

		@return : The absolute distance between a point and the plane
	*/
	virtual float dist_to_point(glm::vec3 const& pt);
	/*
		@function dist_to_point_signed gives the oriented distance between a point and the plane

		@param point : The point from which to calculate the signed distance

		@return : The signed distance between a point and the plane
	*/
	float dist_to_point_signed(glm::vec3 const& point);

	/*
		@function line_intersection gives the point of intersection between a line and the plane

		@param point : 	The starting point of the line
		@param vec : 	The direction of the line

		@return : The interesction point between a point and the plane
	*/
	glm::vec3 line_intersection(glm::vec3 const& point, glm::vec3 const& vec);

	glm::vec2 to_planePos(glm::vec3 const& pt);
	glm::vec2 to_planePos(Point_abstr& pt) { return to_planePos(pt.pos()); }
	glm::vec2 to_planePos(Point_abstr_ptr pt) { return to_planePos(pt->pos()); }
	glm::vec3 to_worldPos(glm::vec2 const& pt);
	glm::vec3 to_worldPos(Geom2d::Point_abstr& pt) { return to_worldPos(pt.pos()); }
	glm::vec3 to_worldPos(Geom2d::Point_abstr_ptr pt) { return to_worldPos(pt->pos()); }


	/*
		@function inverted tells if the plane has a manual inversion applied

		@return : Whether or not the plane has a manual inversion
	*/
	bool inverted() const { return mDir < 0.0f; };

	/*
		@function origin

		@return : The origin of the plane
	*/
	glm::vec3 origin() const { return mOrigin; }
	/*
		@function v

		@return : One of the two vectors that lie on the plane without being parallel
	*/
	glm::vec3 v() const { return mV; }
	/*
		@function v

		@return : One of the two vectors that lie on the plane without being parallel
	*/
	glm::vec3 w() const { return mW; }
};

} // !Geom3d

#endif