
#ifndef HOME_HPP_
#define HOME_HPP_

#include "workspace.hpp"

#include <map>

class home : public workspace {
private:
	std::map<std::string, std::pair<Gtk::Button*, Gtk::Image*>> mButtons;
public:
	home(bloop* parent);
	home(Glib::RefPtr<Gtk::Builder> const& builder, bloop* parent);

	void new_file();
	void open_file();
};

#endif