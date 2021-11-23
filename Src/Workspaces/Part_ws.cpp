
#include "Part_ws.hpp"

#include <Utils/Expunge.hpp>
#include <Bloop/Bloop.hpp>
#include <Tools/Part/PartDefault_tool.hpp>
#include <Tools/Part/StartSketch_tool.hpp>
#include <Tools/Navigation3d/Navigation3d.hpp>

#include <memory>

Part_ws::Part_ws(Glib::RefPtr<Gtk::Builder> const& builder, Bloop* parent) :
	Workspace_abstr("Part_ws_upperBar", builder, parent) // Create base workspace with ui upper bar
{
	// Creat all the tools used in this workspace
	mPartDefault_tool 		= new PartDefault_tool(this);
	mStartSketch_tool 		= new StartSketch_tool(this);
	mPan3d_tool				= new Pan3d_tool(this);
	mDefaultTool = mPartDefault_tool;

	// Initialize all buttons as 2 nullptr
	mButtons["startSketch"] = std::make_pair<Gtk::Button*, Gtk::Image*>(nullptr, nullptr);
	mButtons["extrusion"] 	= std::make_pair<Gtk::Button*, Gtk::Image*>(nullptr, nullptr);

	// Load all buttons from the builder
	bool all_btns_valid = true;
	for(auto it = mButtons.begin(); it != mButtons.end(); ++it) {
		builder->get_widget(it->first, std::get<0>(it->second));

		if(!std::get<0>(it->second))
			all_btns_valid = false;
	}

	if(all_btns_valid) {
		try { // Attempt to fetch all the buttons' icons
			std::get<1>(mButtons.at("startSketch")) = new Gtk::Image(Gdk::Pixbuf::create_from_file("Resources/Textures/Images/Icons/Part/Buttons/StartSketch_button.png", 60, 60));
			std::get<1>(mButtons.at("extrusion")) 	= new Gtk::Image(Gdk::Pixbuf::create_from_file("Resources/Textures/Images/Icons/Part/Buttons/Extrusion_button.png", 60, 60));
		} catch(const Glib::FileError& ex) {
			LOG_WARNING("Glib file error: " + ex.what());
		} catch(const Gdk::PixbufError& ex) {
			LOG_WARNING("Gtk pixbuf error: " + ex.what());
		}
		// Set all the buttons' icons		
		std::get<0>(mButtons.at("startSketch"))->	set_image(*std::get<1>(mButtons.at("startSketch")));
		std::get<0>(mButtons.at("extrusion"))->		set_image(*std::get<1>(mButtons.at("extrusion")));

		// Set all the buttons' callback
		std::get<0>(mButtons.at("startSketch"))->	signal_clicked().connect(sigc::mem_fun(*this, &Part_ws::begin_startSketch));
		std::get<0>(mButtons.at("extrusion"))->		signal_clicked().connect(sigc::mem_fun(*this, &Part_ws::begin_extrusion));
	} else {
		LOG_ERROR("Could not build part design Workspace_abstr.");
	}
}
Part_ws::~Part_ws()
{
	expunge(mPartDefault_tool);
	expunge(mStartSketch_tool);
	mDefaultTool = nullptr;

	for(auto it = mButtons.begin(); it != mButtons.end(); ++it) {
		expunge(it->second.first);
		expunge(it->second.second);
	}
}

bool Part_ws::set_tool(int name)
{
	Tool_abstract* to_set = nullptr;
	switch(name) {
	case TOOLIDS::TOOLID_PARTDESIGNDEFAULT:
		to_set = mPartDefault_tool;
		break;
	case TOOLIDS::TOOLID_STARTSKECTH:
		to_set = mStartSketch_tool;
		break;
	case TOOLIDS::TOOLID_PAN:
		to_set = mPan3d_tool;
		break;
	default:
		return Workspace_abstr::set_tool(name);
	}
	Workspace_abstr::set_tool(to_set);
	return true;
}

bool Part_ws::manage_key_press(GdkEventKey* event)
{
	switch(event->keyval) {
	case GDK_KEY_S:
	case GDK_KEY_s:
		begin_startSketch();
		break;
	default:
		return Workspace_abstr::manage_key_press(event);
	}
	return true;
}

void Part_ws::begin_startSketch()
{
	set_tool(TOOLID_STARTSKECTH);
}
void Part_ws::begin_extrusion()
{
	LOG_WARNING("Extrusion not implemented yet");
}