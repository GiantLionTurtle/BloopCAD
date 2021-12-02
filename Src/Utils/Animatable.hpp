
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

#ifndef ANIMATABLE_HPP_
#define ANIMATABLE_HPP_

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <chrono>

/*
	@class Animatable describes a way to animate a variable over time, currently, it only does linear animations

	@note : It will likely will be reworked to have a nicer API and access in general
*/
template<typename eT>
class Animatable {
private:
	eT mVal; // Actual stored value
	eT mInitVal, mTargetVal; // Value at the start and the end of the animation
	int mAnimationDuration; // Duration of the animation
	std::chrono::steady_clock::time_point mAnimationEnd; // Time stamp of the animation end
	bool mAnimationEnded; // If the animation is done
public:
	/*
		@function Animatable creates an Animatable object with an init value

		@param val : The initial value of the variable
	*/
	Animatable(eT val)
		: mVal(val)
		, mAnimationEnded(true)
	{}
	/*
		@function set simply sets the variable's value, without animation 

		@param val : The value to assign
	*/
	void set(eT val)
	{
		mAnimationEnded = true;
		mVal = val;
	}
	/*
		@function set simply sets the variable's value, over a time period 

		@param val : 			The value to assign
		@param duration_ms : 	The length of the animation in ms
	*/	
	void set(eT val, unsigned int duration_ms)
	{
		if(duration_ms == 0) // Animation of 0ms is the same as just setting
			set(val);
	
		mAnimationEnded = false; // It is just the begining!
		mTargetVal = val;
		mInitVal = mVal;
		mAnimationDuration = duration_ms;
		mAnimationEnd = std::chrono::steady_clock::now(); // Kind of sketch, there might
		mAnimationEnd += std::chrono::milliseconds(duration_ms); // be a better way to make a time point duration_ms in the future
	}
	/*
		@function set simply sets the variable's value, over a time period spending from now to a point in time

		@param val : 	The value to assign
		@param until : 	The point in time when the animation will end
	*/	
	void set(eT val, std::chrono::steady_clock::time_point until)
	{
		mAnimationEnded = false;
		mTargetVal = val;
		mInitVal = mVal;
		mAnimationDuration = std::chrono::duration_cast<std::chrono::milliseconds>(until - std::chrono::steady_clock::now()).count();
		if(mAnimationDuration <= 0) 
			set(val);
		mAnimationEnd = until;
	}
	/*
		@function update updates the animation if it isn't ended
	*/
	void update()
	{
		if(steady()) // No need to update
			return;
		
		float timeLeft = std::chrono::duration_cast<std::chrono::milliseconds>(mAnimationEnd - std::chrono::steady_clock::now()).count();
		if(timeLeft > 0) {
			// Simple linear interpolation
			float amount = 1.0f - timeLeft / mAnimationDuration;
			update_internal(amount);
		} else {
			// Just set to prevent rounding errors
			mVal = mTargetVal;
			mAnimationEnded = true;
		}
	}
	
	/*
		@function steady tells if the animation has ended 
	*/
	bool steady() const
	{
		return mAnimationEnded;
	}

	/*
		@function get grants access to a reference to the variable

		@return : A reference to the variable
	*/
	eT& get() { return mVal; };
	/*
		@function get grants access to a copy of the variable

		@return : A copy of the variable
	*/
	eT get() const { return mVal; };
	/*
		@operator eT casts the object into it's variable type by copying it's variable
	*/
	operator eT()  { return mVal; };

	/*
		@function *= applies the operator to the variable

		@param thing : The right operand

		@note : Wack
	*/
	template<typename oT>
	eT operator *=(oT thing)
	{
		return mVal *= thing;
	}
	/*
		@function += applies the operator to the variable

		@param thing : The right operand

		@note : wack
	*/
	template<typename oT>
	eT operator +=(oT thing)
	{
		return mVal += thing;
	}
private:
	/*
		@function update_internal updates the value at some percentage 

		@param amount : Where on the lerp should the value be
	*/
	void update_internal(float amount)
	{
		mVal = mInitVal + (mTargetVal - mInitVal) * amount;
	}
};

/*
	@function Animatable is a specialization of the update_internal method for quaternions

	@param amount : Where on the lerp should the value be
*/
template<> void Animatable<glm::quat>::update_internal(float amount);

#endif