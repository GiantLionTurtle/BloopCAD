
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

#endif