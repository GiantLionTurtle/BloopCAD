
#include "Tool.hpp"
#include <Utils/Debug_util.hpp>

void Tool_abstract::load_icon(std::string const& loc)
{
    // Attempt to load the cursor icon
	try {
		mCursor = Gdk::Cursor::create(Gdk::Display::get_default(), Gdk::Pixbuf::create_from_file(loc, 24, 24), 7, 6);
	} catch(const Glib::FileError& ex) {
		LOG_WARNING("Glib::FileError: " + ex.what());
	} catch(const Gdk::PixbufError& ex) {
		LOG_WARNING("Glib::PixbufError: " + ex.what());
	}
}
