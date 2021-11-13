
#ifndef BASEOBJECT_HPP_
#define BASEOBJECT_HPP_

#include <memory>

class BaseObject {
public:
	virtual bool exists() const = 0;
	virtual void set_exists(bool exists_) = 0;
};

class DefaultBaseObject : public BaseObject {
protected:
	bool mExists;
public:
	DefaultBaseObject(): mExists(true) {}
	virtual ~DefaultBaseObject() {}
	virtual bool exists() const { return mExists; }
	virtual void set_exists(bool exists_) { mExists = exists_; }
};


class UI_Core_Link {
public:
	enum MsgStd { LNK_DEAD = 1, LNK_ALIVE = 2, LNK_HOVER = 3, LNK_UNHOVER = 4};
protected:
	UI_Core_Link* mLinkTo { nullptr };
public:
	UI_Core_Link() = default;
	UI_Core_Link(UI_Core_Link* linkto)
		: mLinkTo(linkto)
	{

	}

	void set_linkTo(UI_Core_Link* linkto)
	{
		mLinkTo = linkto;
	}
	void send(int msg)
	{
		if(valid())
			mLinkTo->receive(msg, this);
	}
	virtual void receive(int msg, UI_Core_Link* sender) = 0;

	bool valid()
	{
		return mLinkTo != nullptr;
	}
};

#endif