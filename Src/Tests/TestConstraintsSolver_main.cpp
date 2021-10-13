

int main()
{
	return 0;
}

// // #include <ConstraintsSolver/DR_planner.hpp>
// #include <Utils/Debug_util.hpp>
// #include <Utils/Preferences.hpp>
// #include <Utils/Conversions_util.hpp>
// #include <ConstraintsSolver/Constraint_abstr.hpp>
// #include <ConstraintsSolver/ConstraintsSystem.hpp>
// #include <testFramework_EventsManager.hpp>

// #include "Bloop.hpp"
// #include "Workspaces/Part_ws.hpp"

// #include <gtkmm.h>

// #include <iostream>
// #include <iomanip>
// #include <cmath>

// void printLine(sketchLine_ptr l, std::string pre = "")
// {
// 	using std::setw;
// 	int w = 20;
// 	std::cout<<pre<<"("<<setw(w)<<l->A()->x()->eval()<<",  "<<setw(w)<<l->A()->y()->eval()<<") ------ ("
// 				<<setw(w)<<l->B()->x()->eval()<<",  "<<setw(w)<<l->B()->y()->eval()<<")\n";
// }

// int main(int argc, char *argv[])
// {
// 	ErrorLogger::get_instance().init(); // Init the error logger singleton
// 	Preferences::get_instance().load_from_file("Resources/Configs/Configs.xml"); // Init the Preferences service singleton

// 	auto app = Gtk::Application::create(argc, argv, ""); // Create an application 
// 	auto refBuilder = Gtk::Builder::create(); // Gtk builder
// 	try {
// 		refBuilder->add_from_file("Resources/GUI/Layout.glade");
// 	} catch(const Glib::FileError& ex) {
// 		LOG_ERROR("Glib file error: " + ex.what());
// 		return 1;
// 	} catch(const Glib::MarkupError& ex) {
// 		LOG_ERROR("Glib markup error: " + ex.what());
// 		return 1;
// 	} catch(const Gtk::BuilderError& ex) {
// 		LOG_ERROR("Gtk builder error: " + ex.what());
// 		return 1;
// 	}

// 	Bloop* window;
// 	refBuilder->get_widget_derived("Bloop", window);

// 	Document* doc = std::make_shared<Document>(new Test_EventsManager);
// 	doc->set_name("test_doc");
// 	window->add_document(doc);

// 	if(window) {
// 		return app->run(*window); // Run the app
// 	}

// 	LOG_ERROR("Could not build window, returning early.");



// 	return 0;
// }


// /*
// 	glm::vec2 A(0.0f, 0.0f), B(0.5f, -1.0f), C(1.0f, 0.0f), D(0.5f, 1.0f);
// 	Geom3d::Plane_abstr* pl = new Geom3d::Plane_abstr(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

// 	sketchLine_ptr l1 = std::make_shared<sketchLine>(A, B, pl);
// 	sketchLine_ptr l2 = std::make_shared<sketchLine>(B, C, pl);
// 	sketchLine_ptr l3 = std::make_shared<sketchLine>(C, D, pl);
// 	sketchLine_ptr l4 = std::make_shared<sketchLine>(D, A, pl);

// 	ConstraintsSystem system;
	
// 	system.add_constraint(pointPoint_distance::make_coincident(l1->B(), l2->A()));
// 	system.add_constraint(pointPoint_distance::make_coincident(l2->B(), l3->A()));
// 	system.add_constraint(pointPoint_distance::make_coincident(l3->B(), l4->A()));
// 	system.add_constraint(pointPoint_distance::make_coincident(l4->B(), l1->A()));

// 	system.add_constraint(SkPointPoint_horizontality::make(l2));
// 	system.add_constraint(lineLine_angle::make_perpendicular(l2, l1));
// 	system.add_constraint(lineLine_angle::make_perpendicular(l2, l3));
// 	system.add_constraint(lineLine_angle::make_perpendicular(l3, l4));

// 	system.solve();

// 	printLine(l1, "l1 ");
// 	printLine(l2, "l2 ");
// 	printLine(l3, "l3 ");
// 	printLine(l4, "l4 ");

// */

// // int main()
// // {
// // 	glm::vec2 A(0.0f, -1.0f), B(0.5f, -1.0f), C(1.0f, 0.0f);
// // 	Geom3d::Plane_abstr* pl = new Geom3d::Plane_abstr(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

// // 	sketchLine_ptr l1 = std::make_shared<sketchLine>(A, B, pl);
// // 	sketchLine_ptr l2 = std::make_shared<sketchLine>(B, C, pl);

// // 	ConstraintsSystem system;
	
// // 	// system.add_constraint(SkPointPoint_coincidence::make(l1->B(), l2->A()));

// // 	// system.add_constraint(SkPointPoint_horizontality::make(l1));
// // 	// system.add_constraint(lineLine_angle::make_perpendicular(l2, l1));
// // 	// system.add_constraint(lineLine_angle_reduced::make_perpendicular(l2, l1));

// // 	system.solve();

// // 	printLine(l1, "l1 ");
// // 	printLine(l2, "l2 ");
// // }