
#include <Utils/Debug_util.hpp>
#include <Utils/Preferences.hpp>
#include <Utils/Conversions_util.hpp>

#include "Bloop.hpp"
#include "Workspaces/Part_ws.hpp"

#include <gtkmm.h>
#include <fontconfig/fontconfig.h>

#include <glm/glm.hpp>
#include <glm/ext/quaternion_common.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <memory>
#include <vector>
#include <tuple>
#include <map>

// http://gtk.10911.n7.nabble.com/Trying-hard-with-a-custom-cellrenderer-td45721.html
// https://stackoverflow.com/questions/42115586/how-to-add-a-hover-effect-to-a-gtktreeview-cell
int main(int argc, char *argv[])
{
	ErrorLogger::get_instance().init(); // Init the error logger singleton
	Preferences::get_instance().load_from_file("resources/configs/configs.xml"); // Init the Preferences service singleton

	auto app = Gtk::Application::create(argc, argv, ""); // Create an application 
	auto refBuilder = Gtk::Builder::create(); // Gtk builder
	try {
		refBuilder->add_from_file("resources/GUI/design_documents.glade");
	} catch(const Glib::FileError& ex) {
		LOG_ERROR("Glib file error: " + ex.what());
		return 1;
	} catch(const Glib::MarkupError& ex) {
		LOG_ERROR("Glib markup error: " + ex.what());
		return 1;
	} catch(const Gtk::BuilderError& ex) {
		LOG_ERROR("Gtk builder error: " + ex.what());
		return 1;
	}

	Bloop* window;
	refBuilder->get_widget_derived("Bloop", window);
	
	Document* doc = new Document();
	doc->set_name("Document");
	window->add_document(doc);

	if(window) {
		return app->run(*window); // Run the app
	}

	LOG_ERROR("Could not build window, returning early.");
}

// g++ `pkg-config gtkmm-3.0 --cflags` main.cpp helloworld.cpp -o demo `pkg-config gtkmm-3.0 --libs`