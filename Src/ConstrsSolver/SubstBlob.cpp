
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

#include "SubstBlob.hpp"

#include <Utils/Debug_util.hpp>

SubstBlob::SubstBlob(Param* p)
	: mParams({p})
{

}
SubstBlob::SubstBlob(std::vector<Param*> const& params)
	: mParams(params)
{

}
SubstBlob::SubstBlob(SubstBlob const& rhs)
{
	BLOOP_MARKER;
	mParams = rhs.mParams;

	for(auto p : mParams) {
		p->set_blob(this);
	}
}
SubstBlob& SubstBlob::operator=(SubstBlob const& rhs)
{
	BLOOP_MARKER;
	clear();
	mParams = rhs.mParams;

	for(auto p : mParams) {
		p->set_blob(this);
	}
	return *this;
}

void SubstBlob::clear()
{
	for(auto p : mParams) {
		p->set_blob(nullptr);
	}
	mParams.clear();
}

void SubstBlob::choose_driving()
{
	int max_weight = 0;

	for(auto p : mParams) {
		if(p->weight() > max_weight) {
			max_weight = p->weight();
			mVal = p->val_internal();
		}
	}
	if(max_weight == 0) {
		LOG_WARNING("Problem.");
	}
	set_frozen(max_weight);
	for(auto p : mParams) {
		p->set_origin(&mVal);
	}
}
void SubstBlob::update_params()
{
	for(auto p : mParams) {
		p->reset_origin(true);
	}
}
void SubstBlob::absorb(SubstBlob const& blob)
{
	for(auto p : blob.mParams) {
		add_param(p);
	}
}
void SubstBlob::add_param(Param* p)
{
	p->set_blob(this);
	mParams.push_back(p);
}
