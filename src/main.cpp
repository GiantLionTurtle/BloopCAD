
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


struct st {
	std::string name;
	st(std::string const& str) :
		name(str)
	{}
};


int main (int argc, char *argv[])
{
	/*
		Init: 5.173954, 4.249190, 4.378881
		Delta: vec2(-0.010894, -0.003804)
		Predicted pos[1]: vec3(5.221350, 4.249190, 4.322257)
		Rot: quat(0.871874, {0.250690, -0.404320, -0.116254})
		Actual pos: vec3(5.229948, 4.248937, 4.312097)
	*/
	// glm::quat q1 = glm::angleAxis((float)M_PI/4.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	// glm::quat q2 = glm::angleAxis(-(float)M_PI/4.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	// glm::quat q3 = glm::angleAxis(-(float)M_PI/4.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	// glm::vec3 pos(0.0f, 0.0f, 8.0f);
	// // glm::vec3 pos(4.6188f, 4.6188f, 4.6188f);
	
	// glm::vec3 p_rot_1 = glm::inverse(glm::toMat3(q1)) * pos;
	// glm::vec3 p_rot_2 = glm::inverse(glm::toMat3(q2 * q1)) * pos;
	// glm::vec3 p_rot_3 = glm::inverse(glm::toMat3(q3 * q2 * q1)) * pos;
	// std::cout<<"p_rot: \n"<<glm::to_string(pos)<<"=>\n"<<glm::to_string(p_rot_1)<<"=>\n"<<glm::to_string(p_rot_2)<<"=>\n"<<glm::to_string(p_rot_3)<<"\n";
	
	// glm::vec3 camFront(0.0f, 0.0f, 1.0f);
	// glm::vec3 targetFront(0.0f, 0.0f, 1.0f);

	// glm::vec2 spherePos(
	// 	M_PI_2 - std::atan2(targetFront.z, targetFront.x),
	// 	M_PI_2 - std::atan2(std::sqrt(targetFront.x*targetFront.x + targetFront.z*targetFront.z), targetFront.y)
	// );
	// std::cout<<"spherePos: "<<glm::to_string(spherePos)<<"\n";
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