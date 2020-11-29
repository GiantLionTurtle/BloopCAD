
#include "partDesign.hpp"
#include <bloop.hpp>

#include <tools/partDesign/startSketch_tool.hpp>

partDesign::partDesign(bloop* parent) :
	workspace(parent)
{}

partDesign::partDesign(Glib::RefPtr<Gtk::Builder> const& builder, bloop* parent) :
	workspace("partDesign_upperBar", builder, parent)
{
	mTools["startSketch"] 	= std::shared_ptr<tool_abstract>(new startSketch_tool(this));
	mTools["extrusion"] 	= std::shared_ptr<tool_abstract>(new extrusion_tool(this));
	mDefaultTool = mTools.at("simpleSelector");

	mButtons["startSketch"] = std::make_pair<Gtk::Button*, Gtk::Image*>(nullptr, nullptr);
	mButtons["extrusion"] 	= std::make_pair<Gtk::Button*, Gtk::Image*>(nullptr, nullptr);

	bool all_btns_valid = true;
	for(auto it = mButtons.begin(); it != mButtons.end(); ++it) {
		builder->get_widget(it->first, std::get<0>(it->second));

		if(!std::get<0>(it->second))
			all_btns_valid = false;
	}

	if(all_btns_valid) {
		try {
			std::get<1>(mButtons.at("startSketch")) = new Gtk::Image(Gdk::Pixbuf::create_from_file("resources/textures/images/icons/partDesign/buttons/startSketch_button.png", 60, 60));
			std::get<1>(mButtons.at("extrusion")) 	= new Gtk::Image(Gdk::Pixbuf::create_from_file("resources/textures/images/icons/partDesign/buttons/extrusion_button.png", 60, 60));
		} catch(const Glib::FileError& ex) {
			LOG_WARNING("Glib file error: " + ex.what());
		} catch(const Gdk::PixbufError& ex) {
			LOG_WARNING("Gtk pixbuf error: " + ex.what());
		}
		
		std::get<0>(mButtons.at("startSketch"))->	set_image(*std::get<1>(mButtons.at("startSketch")));
		std::get<0>(mButtons.at("extrusion"))->		set_image(*std::get<1>(mButtons.at("extrusion")));

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
	// set_tool_target<part>("extrusion");
	// mParentBloop->currentDocument()->set_tool(mTools.at("extrusion"));
	// mParentBloop->set_tool("extrusion");

	LOG_WARNING("Extrusion not implemented yet");
}