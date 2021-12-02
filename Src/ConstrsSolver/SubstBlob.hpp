
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

#ifndef SUBSTNBLOB_HPP_
#define SUBSTNBLOB_HPP_

#include <Utils/Param.hpp>

#include <vector>

/*
	A class that represents one or more parameters that have 
	a substitution constraint applied on them (pointpointcoincidence, verticality, horizontality)

	Simplifies work on constraint graph analysis
*/
class SubstBlob : public Param {
private:
	std::vector<Param*> mParams;
public:
	SubstBlob(Param* p);
	SubstBlob(std::vector<Param*> const& params);
	SubstBlob() = default;
	SubstBlob(SubstBlob const& rhs);
	SubstBlob& operator=(SubstBlob const& rhs);

	void clear();

	void choose_driving();
	void update_params();
	void absorb(SubstBlob const& blob);
	void add_param(Param* p);
	void release_params();
};

#endif