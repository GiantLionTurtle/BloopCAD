
#ifndef DRAGGEDPOINT_HPP_
#define DRAGGEDPOINT_HPP_

#include "Constraint_abstr.hpp"

#include <glm/glm.hpp>

#include <array>

class DragSystemHandler {
	virtual void dragUpdate() = 0;

	constexpr size_t size() { return 2; }
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