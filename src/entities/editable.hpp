
#ifndef EDITABLE_HPP_
#define EDITABLE_HPP_

class editable {
public:
	virtual bool is_sketch() { return false; }
	virtual bool is_part() { return false; }
};

#endif