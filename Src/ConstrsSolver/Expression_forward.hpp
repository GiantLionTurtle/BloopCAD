
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

#ifndef EXPRESSION_FORWARD_HPP_
#define EXPRESSION_FORWARD_HPP_

#include <memory>

class Expression_abstr;
class ExpVar;
class ExpEqu;
using exp_ptr = std::shared_ptr<Expression_abstr>; // TODO: change to exp_ptr
using var_ptr = std::shared_ptr<ExpVar>;
using equ_ptr = std::shared_ptr<ExpEqu>;

#endif