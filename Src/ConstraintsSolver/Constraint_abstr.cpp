
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

#include "Constraint_abstr.hpp"

#include "Expression.hpp"

Constraint_abstr::Constraint_abstr()
{

}
Constraint_abstr::~Constraint_abstr()
{

}
double Constraint_abstr::error(size_t ind)
{
    return equ(ind)->eval();
}
bool Constraint_abstr::satisfied()
{
    for(size_t i = 0; i < n_equs(); ++i) {
        if(std::abs(equ(i)->eval()) > 1e-12)
            return false;
    }
    return true;
}
double Constraint_abstr::derivative(var_ptr withRespectTo, size_t from_eq)
{
    // TODO: would going through the constraint's variables to check if the target
    // is in the constraint save time? ; output zero if the variable is unknown to this constraint
    withRespectTo->set_as_var();
    double der = equ(from_eq)->d()->eval();
    withRespectTo->set_as_coeff();
    return der;
}