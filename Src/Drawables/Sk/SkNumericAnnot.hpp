

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

#ifndef SKNUMERICANNOT_HPP_
#define SKNUMERICANNOT_HPP_

#include "SkDrawable.hpp"
#include <ConstrsSolver/Constraint.hpp>
#include <Utils/Units_utils.hpp>

class SkNumericAnnot : public SkIntDrawable {
protected:
	double mVal;
	bool mBuilt_constr { false };
public:
	SkNumericAnnot(Geom3d::Plane_abstr* pl);

	Constraint* build_constr();

	inline double val() { return mVal; }
	void set_val(double val);

	virtual void moveTo(glm::vec2 pos) = 0;

	virtual int expected_unit() = 0;
protected:
	virtual Constraint* build_constr_impl() = 0;
	virtual void update_constr() = 0;
};

#endif