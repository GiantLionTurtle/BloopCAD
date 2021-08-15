
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

	std::array<var_ptr, dims> mVars;
	std::vector<equ_ptr> mEqus;

	bool mExists;
public:
	DraggedPoint(DragSystemHandler* emiter):
		mPointEmiter(emiter),
		mExists(true)
	{
		for(size_t i = 0; i < dims; ++i) {
			mVars[i] = ExpVar::make(0.0);	
		}
	}

	void dragTo(glm::vec<dims, float, glm::defaultp> pos)
	{
		for(size_t i = 0; i < dims; ++i) {
			mVars[i]->set(*(&pos[0]+i));
		}
	}
	template<typename eT>
	void set_equality(ExpVec<eT, dims> equality)
	{
		mEqus.resize(dims);
		mPointEmiter->dragUpdate();
		
		for(size_t i = 0; i < dims; ++i) {
			mEqus[i] = mVars[i] ^= equality.comp(i);
		}
	}
	void set_equality(std::array<exp_ptr, dims> equality)
	{
		mEqus.resize(dims);
		mPointEmiter->dragUpdate();
		
		for(size_t i = 0; i < dims; ++i) {
			mEqus[i] = mVars[i] ^= equality[i];
		}
	}
	void clear()
	{
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
};

#endif