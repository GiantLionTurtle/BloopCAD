
#ifndef SKETCHDESIGN_HPP_
#define SKETCHDESIGN_HPP_

#include "workspace.hpp"

#include <map>

class sketchDesign : public workspace {
private:
	std::map<std::string, std::pair<Gtk::Button*, Gtk::Image*>> mButtons;
public:
	sketchDesign(bloop* parent);
	sketchDesign(Glib::RefPtr<Gtk::Builder> const& builder, bloop* parent);

	void line();
	void rectangle();
	void polygon();
	void circle();
	void threePointsArc();

	void dimension();
	void verticality();
	void horizontality();
	void perpendicularity();
	void parallelism();
	void coincidence();
	void equality();

	void finish();
};

#endif