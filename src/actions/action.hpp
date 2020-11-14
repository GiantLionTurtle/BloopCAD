
#ifndef ACTION_HPP_
#define ACTION_HPP_

class action {
public:
	action() {};
	virtual ~action() {};

	virtual void do_work() = 0;
	virtual void undo_work() = 0;
};

#endif