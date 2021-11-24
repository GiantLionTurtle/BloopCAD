
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

#ifndef DRAGGEDPOINT_HPP_
#define DRAGGEDPOINT_HPP_

#include "Constraint_abstr.hpp"
#include "ExpressionVec.hpp"

#include <glm/glm.hpp>

#include <array>

class DragSystemHandler {
public:
	virtual void dragUpdate() = 0;
};

template<size_t dims>
class DraggedPoint : public Constraint_abstr {
private:
	DragSystemHandler* mPointEmiter;

	std::array<var_ptr, dims> mPoint;
	std::vector<var_ptr> mVars;
	std::vector<equ_ptr> mEqus;

	bool mExists;
public:
	DraggedPoint(DragSystemHandler* emiter):
		mPointEmiter(emiter),
		mExists(false)
	{
		for(size_t i = 0; i < dims; ++i) {
			mPoint[i] = ExpVar::make(0.0);	
		}
	}

	void dragTo(glm::vec<dims, float, glm::defaultp> pos)
	{
		for(size_t i = 0; i < dims; ++i) {
			// TODO: change to drag()??
			mPoint[i]->set(*(&pos[0]+i));
		}
	}
	template<typename eT>
	void set_equality(ExpVec<eT, dims> equality, std::vector<var_ptr> vars)
	{
		set_exists(true);
		mEqus.resize(dims);
		mVars = vars;
		mPointEmiter->dragUpdate();

		for(size_t i = 0; i < dims; ++i) {
			mEqus[i] = mPoint[i] ^= equality.comp(i);
		}
	}
	// void set_equality(std::array<exp_ptr, dims> equality)
	// {
	// 	set_exists(true);
	// 	mEqus.resize(dims);
	// 	mPointEmiter->dragUpdate();
		
	// 	for(size_t i = 0; i < dims; ++i) {
	// 		mEqus[i] = mVars[i] ^= equality[i];
	// 	}
	// }
	void clear()
	{
		set_exists(false);
		mEqus.clear();
		mPointEmiter->dragUpdate();
	}
	bool valid()
	{
		return !mEqus.empty();
	}
	operator bool() { return valid(); } // !!! Important, can't be used on pointer type
	equ_ptr equ(size_t ind)
	{
		return mEqus[ind];
	}
	size_t n_equs()
	{
		return mEqus.size();
	}

	var_ptr var(size_t ind)
	{
		return mVars[ind];
	}
	size_t n_vars()
	{
		return mVars.size();
	}

	std::string name() { return "DraggedPoint"; }

	virtual bool exists() const { return mExists; }
    virtual void set_exists(bool exists_) { mExists = exists_; }

	glm::vec2 pos()
	{
		return glm::vec2(mVars[0]->eval(), mVars[1]->eval());
	}
};

#endif