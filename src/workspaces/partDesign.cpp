
#include "partDesign.hpp"

#include <bloop.hpp>
#include <tools/partDesign/partDesignDefault_tool.hpp>
#include <tools/partDesign/startSketch_tool.hpp>
#include <entities/part.hpp>

#include <memory>

partDesign::partDesign(bloop* parent) :
	workspace(parent)
{}

partDesign::partDesign(Glib::RefPtr<Gtk::Builder> const& builder, bloop* parent) :
	workspace("partDesign_upperBar", builder, parent) // Create base workspace with ui upper bar
{
	// Creat all the tools used in this workspace
	mTools["startSketch"] 	= tool_abstract_ptr(new startSketch_tool(this));
	mTools["default"]		= tool_abstract_ptr(new partDesignDefault_tool(this));
	// mTools["extrusion"] 	= tool_abstract_ptr(new extrusion_tool(this));
	mDefaultTool = mTools.at("default");

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
			std::get<1>(mButtons.at("startSketch")) = new Gtk::Image(Gdk::Pixbuf::create_from_file("resources/textures/images/icons/partDesign/buttons/startSketch_button.png", 60, 60));
			std::get<1>(mButtons.at("extrusion")) 	= new Gtk::Image(Gdk::Pixbuf::create_from_file("resources/textures/images/icons/partDesign/buttons/extrusion_button.png", 60, 60));
		} catch(const Glib::FileError& ex) {
			LOG_WARNING("Glib file error: " + ex.what());
		} catch(const Gdk::PixbufError& ex) {
			LOG_WARNING("Gtk pixbuf error: " + ex.what());
		}
		// Set all the buttons' icons		
		std::get<0>(mButtons.at("startSketch"))->	set_image(*std::get<1>(mButtons.at("startSketch")));
		std::get<0>(mButtons.at("extrusion"))->		set_image(*std::get<1>(mButtons.at("extrusion")));

		// Set all the buttons' callback
		std::get<0>(mButtons.at("startSketch"))->	signal_clicked().connect(sigc::mem_fun(*this, &partDesign::startSketch));
		std::get<0>(mButtons.at("extrusion"))->		signal_clicked().connect(sigc::mem_fun(*this, &partDesign::extrusion));
	} else {
		LOG_ERROR("Could not build part design workspace.");
	}
}

void partDesign::startSketch()
{
	set_tool("startSketch");
}
void partDesign::extrusion()
{
	LOG_WARNING("Extrusion not implemented yet");
}