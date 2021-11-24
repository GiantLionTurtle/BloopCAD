
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