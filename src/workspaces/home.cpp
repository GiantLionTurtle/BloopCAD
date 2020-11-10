
#include "home.hpp"
#include "../bloop.hpp"

home::home(bloop* parent) :
	workspace(parent)
{}

home::home(Glib::RefPtr<Gtk::Builder> const& builder, bloop* parent) :
	workspace("home_upperBar", builder, parent)
{
	mButtons["new"]     = std::make_pair<Gtk::Button*, Gtk::Image*>(nullptr, nullptr);
	mButtons["open"]    = std::make_pair<Gtk::Button*, Gtk::Image*>(nullptr, nullptr);

	bool all_btns_valid = true;
	for(auto it = mButtons.begin(); it != mButtons.end(); ++it) {
		builder->get_widget(it->first, std::get<0>(it->second));

		if(!std::get<0>(it->second))
			all_btns_valid = false;
	}

	if(all_btns_valid) {
		try {
			std::get<1>(mButtons.at("new"))     = new Gtk::Image(Gdk::Pixbuf::create_from_file("resources/textures/images/icons/home/new.png", 60, 60));
			std::get<1>(mButtons.at("open"))    = new Gtk::Image(Gdk::Pixbuf::create_from_file("resources/textures/images/icons/home/open.png", 60, 60));
		} catch(const Glib::FileError& ex) {
			LOG_ERROR("Glib file error: " + ex.what());
		} catch(const Gdk::PixbufError& ex) {
			LOG_ERROR("Gtk pixbuf error: " + ex.what());
		}
		
		std::get<0>(mButtons.at("new"))->	set_image(*std::get<1>(mButtons.at("new")));
		std::get<0>(mButtons.at("open"))->	set_image(*std::get<1>(mButtons.at("open")));

		std::get<0>(mButtons.at("new"))->signal_clicked().connect(sigc::mem_fun(*this, &home::new_file));
		std::get<0>(mButtons.at("open"))->signal_clicked().connect(sigc::mem_fun(*this, &home::open_file));
	} else {
		LOG_ERROR("Could not build sketch workspace.");
	}
}

void home::new_file()
{
	LOG_WARNING("New file not implemented yet");
}
void home::open_file()
{
	LOG_WARNING("Open file not implemented yet");
}