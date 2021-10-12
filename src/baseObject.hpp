
#ifndef BASEOBJECT_HPP_
#define BASEOBJECT_HPP_

#include <memory>

class baseObject;
using baseObject_ptr = std::shared_ptr<baseObject>;

class baseObject {
public:
    virtual bool exists() const = 0;
    virtual void set_exists(bool exists_) = 0;
};

class defaultBaseObject : public baseObject {
protected:
    bool mExists;
public:
    defaultBaseObject(): mExists(true) {}
    virtual ~defaultBaseObject() {}
    virtual bool exists() const { return mExists; }
    virtual void set_exists(bool exists_) { mExists = exists_; }
};

#endif