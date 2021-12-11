
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

#include <catch2/catch.hpp>

#include <Geometry/2d/Line_abstr.hpp>
#include <ConstrsSolver/ConstrSyst.hpp>
#include <Utils/Expunge.hpp>

TEST_CASE("Perpendicularity constraint is tested with LM solver", "[constraints]")
{
	Geom2d::Line* l1 = new Geom2d::Line(glm::vec2(0.5, 0.6), glm::vec2(0.0, 0.0));
	Geom2d::Line* l2 = new Geom2d::Line(glm::vec2(0.0, 0.0), glm::vec2(-0.8, 0.2));

	LineLine_angle* perp = new LineLine_angle(l1, l2, M_PI_2);
	PointPoint_coincidence* coinc = new PointPoint_coincidence(l1->ptB(), l2->ptA());

	ConstrSyst syst(SolverState::algorithm::LevenbergMarquardt);

	syst.add_constr(perp);
	// syst.add_constr(coinc);

	int result = syst.solve();
	REQUIRE(result == SolverState::SUCCESS);

	expunge(l1);
	expunge(l2);
	expunge(perp);
	expunge(coinc);
}


TEST_CASE("Perpendicularity1", "[DogLeg]")
{
	Geom2d::Line* l1 = new Geom2d::Line(glm::vec2(0.5, 0.6), glm::vec2(0.0, 0.0));
	Geom2d::Line* l2 = new Geom2d::Line(glm::vec2(0.0, 0.0), glm::vec2(-0.8, 0.2));

	LineLine_angle* perp = new LineLine_angle(l1, l2, M_PI_2);
	PointPoint_coincidence* coinc = new PointPoint_coincidence(l1->ptB(), l2->ptA());

	ConstrSyst syst(SolverState::algorithm::DogLeg);

	syst.add_constr(perp);
	// syst.add_constr(coinc);

	int result = syst.solve();
	REQUIRE(result == SolverState::SUCCESS);

	expunge(l1);
	expunge(l2);
	expunge(perp);
	expunge(coinc);
}


TEST_CASE("Perpendicularity2", "[DogLeg]")
{
	Geom2d::Line* l1 = new Geom2d::Line(glm::vec2(-0.3, 1.2), glm::vec2(0.0, 0.0));
	Geom2d::Line* l2 = new Geom2d::Line(glm::vec2(0.0, 0.0), glm::vec2(0.1, 1.3));

	LineLine_angle* perp = new LineLine_angle(l1, l2, M_PI_2);
	PointPoint_coincidence* coinc = new PointPoint_coincidence(l1->ptB(), l2->ptA());

	ConstrSyst syst(SolverState::algorithm::DogLeg);

	syst.add_constr(perp);
	syst.add_constr(coinc);

	int result = syst.solve();
	REQUIRE(result == SolverState::SUCCESS);

	expunge(l1);
	expunge(l2);
	expunge(perp);
	expunge(coinc);
}