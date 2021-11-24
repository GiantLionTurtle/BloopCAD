
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

#include <catch2/catch.hpp>

#include <Utils/Maths_util.hpp>

TEST_CASE("Map function is tested", "[maths]")
{
	REQUIRE(map(0.5, 0.0, 1.0, -10.0, 10.0) == 0.0);
	REQUIRE(map(0.0, 0.0, 1.0, -10.0, 10.0) == -10.0);
	REQUIRE(map(7.0, 0.0, 10.0, 5.0, 25.0) == 19.0);
	REQUIRE(map(-5.0, -10.0, 10.0, -10.0, 10.0) == -5.0);
}