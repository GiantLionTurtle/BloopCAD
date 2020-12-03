
#ifndef PARTDESIGN_HPP_
#define PARTDESIGN_HPP_

#include "workspace.hpp"

#include <string>

class partDesign : public workspace {
private:
	std::map<std::string, std::pair<Gtk::Button*, Gtk::Image*>> mButtons;
public:
	partDesign(bloop* parent);
	partDesign(Glib::RefPtr<Gtk::Builder> const& builder, bloop* parent);

	void startSketch();
	void extrusion();
};

#endif