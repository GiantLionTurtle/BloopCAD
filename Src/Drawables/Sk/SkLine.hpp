
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

#ifndef SKLINE_HPP_
#define SKLINE_HPP_

#include "SkHandleCurve.hpp"
#include "SkLineCurve.hpp"

class SkLine : public SkHandleCurve<SkHandleCurve_FixedIndexer<2, SkLineCurve>> {
public:
	SkLine(glm::vec2 ptA_, glm::vec2 ptB_, Geom3d::Plane_abstr* pl, bool fixed_ = false):
		SkHandleCurve({ ptA_, ptB_ }, pl, fixed_)
	{
		set_name("SkLine");
	}
	virtual ~SkLine() {}

	void notify(Drawable* who, int msg, bool child)
	{
		if(msg == SkNotifiers::SET) {
			update();
		}
	}

	SkPoint* ptA() { return handle(0); }
	SkPoint* ptB() { return handle(1); }

	int selection_rank() { return 3; }
protected:
	void internalUpdate_impl()
	{
		verbose(VERBOSE_INNERSTEPS, "Lineupdate drag: "<<ptA()->x()->dragged()<<",  "<<
		ptA()->y()->dragged()<<" : "<<ptB()->x()->dragged()<<",  "<<ptB()->y()->dragged())

		if(ptA()->x()->dragged() && !ptB()->x()->dragged()) {
			ptB()->x()->set_dragged(true);
			verbose(VERBOSE_INNERSTEPS, "Set ptbx");
		} else if(!ptA()->x()->dragged() && ptB()->x()->dragged()) {
			ptA()->x()->set_dragged(true);
			verbose(VERBOSE_INNERSTEPS, "Set ptax");
		}

		if(ptA()->y()->dragged() && !ptB()->y()->dragged()) {
			ptB()->y()->set_dragged(true);
			verbose(VERBOSE_INNERSTEPS, "Set ptby");
		} else if(!ptA()->y()->dragged() && ptB()->y()->dragged()) {
			ptA()->y()->set_dragged(true);
			verbose(VERBOSE_INNERSTEPS, "Set ptay");
		}
	}
};

#endif