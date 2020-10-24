
#include "workspace.hpp"
#include "../entities/part.hpp"

#include <string>

class partDesign : public workspace {
private:
	Gtk::Button* mStartSketch_btn, * mExtrude_btn;
	Gtk::Image* mStartSketch_icon, * mExtrude_icon;
public:
	partDesign(bloop* parent);
	partDesign(Glib::RefPtr<Gtk::Builder> const& builder, bloop* parent);

	void startSketch();
	void extrusion();
};