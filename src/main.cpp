
#include "errorLogger.hpp"

#include <gtkmm.h>

#include <memory>
#include <vector>
#include <tuple>
#include <map>

#include "bloop.hpp"
#include "workspaces/partDesign.hpp"
#include <glm/glm.hpp>
#include <glm/ext/quaternion_common.hpp>
#include <glm/gtx/vector_angle.hpp>


int main (int argc, char *argv[])
{
	errorLogger::get_instance().init();
	auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");
	auto refBuilder = Gtk::Builder::create();
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

	bloop* window;

	refBuilder->get_widget_derived("bloop", window);

	if(window) {
		return app->run(*window);
	}

	LOG_ERROR("Could not build window, returning early.");
}

// g++ `pkg-config gtkmm-3.0 --cflags` main.cpp helloworld.cpp -o demo `pkg-config gtkmm-3.0 --libs`