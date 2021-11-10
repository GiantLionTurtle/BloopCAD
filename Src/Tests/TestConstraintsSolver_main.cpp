
#include <Utils/Debug_util.hpp>
#include <Utils/Preferences.hpp>
#include <Utils/Conversions_util.hpp>
#include <ConstraintsSolver/Constraint_abstr.hpp>
#include <ConstraintsSolver/ConstraintsSystem.hpp>
#include <Workspaces/TestFramework_eventsManager.hpp>

#include <Drawables/Sk/SkLine.hpp>
#include <ConstraintsSolver/SolverState.hpp>

#include <Bloop.hpp>
#include "Workspaces/Part_ws.hpp"

#include <gtkmm.h>

#include <iostream>
#include <iomanip>
#include <cmath>
#include <random>
#include <chrono>

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

// 	Document* doc = new Document(new Test_EventsManager);
// 	doc->set_name("test_doc");
// 	window->add_document(doc);

// 	if(window) {
// 		return app->run(*window); // Run the app
// 	}
// 	LOG_ERROR("Could not build window, returning early.");



// 	return 0;
// }

int main()
{
	// glm::vec2 A(0.0f, -1.0f), B(0.5f, -1.0f), C(1.0f, 0.0f);
	// Geom3d::Plane_abstr* pl = new Geom3d::Plane_abstr(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	// SkLine* l1 = new SkLine(A, B, pl);
	// SkLine* l2 = new SkLine(B, C, pl);

	// ConstraintsSystem system;
	
	// system.add_constraint(new SkPointPoint_coincidence(pl, l1->ptB(), l2->ptA()));
	// system.add_constraint(new SkLineLinePerpendicularity(pl, l1->curve(), l2->curve()));
	// system.solve();
	// std::mt19937_64 rng;
    // // initialize the random number generator with time-dependent seed
    // uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    // rng.seed(42); // Keep this seed intact please
    // // initialize a uniform distribution between -2 and 2
    // std::uniform_real_distribution<double> unif(-2, 2);
    // // ready to generate random numbers

    // const int nSimulations = 1000000;
	// int fails = 0;
	// auto start = std::chrono::steady_clock::now();
    // for (int i = 0; i < nSimulations; i++)
    // {
	// 	l1->ptA()->set(glm::vec2(unif(rng), unif(rng)));
	// 	if(system.solve() != SolverState::solveOutput::SUCCESS)
	// 		fails++;

	// 	l1->ptB()->set(glm::vec2(unif(rng), unif(rng)));
	// 	if(system.solve() != SolverState::solveOutput::SUCCESS)
	// 		fails++;

	// 	l2->ptB()->set(glm::vec2(unif(rng), unif(rng)));
	// 	if(system.solve() != SolverState::solveOutput::SUCCESS)
	// 		fails++;
    // }
	// auto end = std::chrono::steady_clock::now();
	// auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	// std::cout<<nSimulations * 3<<" simulations took "<<duration<<" ms which averages to "<<duration / ((double)nSimulations*3.0)<<" ms per solve\n";
	// std::cout<<"It failed "<<fails<<" times which gives a fail percentage of "<<(double)fails / ((double)nSimulations*3.0) * 100.0<<" %\n";
}