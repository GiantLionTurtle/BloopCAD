
#ifndef ANIMATABLE_HPP_
#define ANIMATABLE_HPP_

#include <chrono>

template<typename eT>
class animatable {
private:
	eT mVal;
	eT mInitVal, mTargetVal;
	unsigned int mAnimationDuration;
	std::chrono::steady_clock::time_point mAnimationEnd;
	bool mAnimationEnded;
public:
	animatable(eT val):
		mVal(val),
		mAnimationEnded(true)
	{}
	void set(eT val)
	{
		mAnimationEnded = true;
		mVal = val;
	}
	void set(eT val, unsigned int duration_ms)
	{
		mAnimationEnded = true;
		mTargetVal = val;
		mInitVal = mVal;
		mAnimationDuration = duration_ms;
		mAnimationEnd = std::chrono::steady_clock::now();
		mAnimationEnd += std::chrono::milliseconds(duration_ms);
	}
	void set(eT val, std::chrono::steady_clock::time_point until)
	{
		mAnimationEnded = true;
		mTargetVal = val;
		mInitVal = mVal;
		mAnimationDuration = std::chrono::duration_cast<std::chrono::milliseconds>(until - std::chrono::steady_clock::now()).count();
		mAnimationEnd = until;
	}

	void update()
	{
		if(steady())
			return;
		float timeLeft = std::chrono::duration_cast<std::chrono::milliseconds>(mAnimationEnd - std::chrono::steady_clock::now()).count();
		if(timeLeft > 0) {
			float amount = 1.0f - timeLeft / mAnimationDuration;
			mVal = mInitVal + (mTargetVal - mInitVal) * amount;
		} else {
			mVal = mTargetVal;
			mAnimationEnded = true;
		}
	}
	bool steady()
	{
		return mAnimationEnded;
	}
	eT& get() { return mVal; };
	eT get() const { return mVal; };
	operator eT()  { return mVal; };

	template<typename oT>
	eT operator *=(oT thing)
	{
		return mVal *= thing;
	}

	template<typename oT>
	eT operator +=(oT thing)
	{
		return mVal += thing;
	}
};

#endif