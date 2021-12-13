

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

#include "SkNumericAnnot.hpp"

SkNumericAnnot::SkNumericAnnot(Geom3d::Plane_abstr* pl)
    : SkIntDrawable(pl)
{
    mType |= Drawable_types::ANNOTATION;
}

Constraint* SkNumericAnnot::build_constr()
{
    mBuilt_constr = true;
    return build_constr_impl();
}

void SkNumericAnnot::set_val(double val)
{
    mVal = val;

    if(mBuilt_constr)
        update_constr();
}