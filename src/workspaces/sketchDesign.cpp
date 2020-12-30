
#include "sketchDesign.hpp"

#include <preferences.hpp>
#include <utils/mathUtils.hpp>
#include <actions/common/switchWorkspace_action.hpp>
#include <actions/common/moveCamera_action.hpp>
#include <tools/sketchDesign/line_tool.hpp>
#include <utils/xmlParser.hpp>
#include <entities/svgEntity.hpp>
#include <bloop.hpp>

sketchDesign::sketchDesign(bloop* parent) :
	workspace(parent)
{}

sketchDesign::sketchDesign(Glib::RefPtr<Gtk::Builder> const& builder, bloop* parent) :
	workspace("sketchDesign_upperBar", builder, parent) // Create base workspace with upper bar
{	
	// Create all the tools used in this workspace
	mTools["line"] 	= std::shared_ptr<tool_abstract>(new line_tool(this));
	mDefaultTool = mTools.at("simpleSelector");

	// Initialize all buttons as 2 nullptr
	mButtons["line"] 			= std::make_pair<Gtk::Button*, Gtk::Image*>(nullptr, nullptr);
	mButtons["rectangle"] 		= std::make_pair<Gtk::Button*, Gtk::Image*>(nullptr, nullptr);
	mButtons["circle"] 			= std::make_pair<Gtk::Button*, Gtk::Image*>(nullptr, nullptr);
	mButtons["polygon"] 		= std::make_pair<Gtk::Button*, Gtk::Image*>(nullptr, nullptr);
	mButtons["threePointsArc"] 	= std::make_pair<Gtk::Button*, Gtk::Image*>(nullptr, nullptr);

	mButtons["dimension"] 			= std::make_pair<Gtk::Button*, Gtk::Image*>(nullptr, nullptr);
	mButtons["verticality"] 		= std::make_pair<Gtk::Button*, Gtk::Image*>(nullptr, nullptr);
	mButtons["horizontality"] 		= std::make_pair<Gtk::Button*, Gtk::Image*>(nullptr, nullptr);
	mButtons["perpendicularity"] 	= std::make_pair<Gtk::Button*, Gtk::Image*>(nullptr, nullptr);
	mButtons["parallelism"] 		= std::make_pair<Gtk::Button*, Gtk::Image*>(nullptr, nullptr);
	mButtons["coincidence"] 		= std::make_pair<Gtk::Button*, Gtk::Image*>(nullptr, nullptr);
	mButtons["equality"] 			= std::make_pair<Gtk::Button*, Gtk::Image*>(nullptr, nullptr);

	mButtons["to_svg"] = std::make_pair<Gtk::Button*, Gtk::Image*>(nullptr, nullptr);

	mButtons["sketchFinish"] = std::make_pair<Gtk::Button*, Gtk::Image*>(nullptr, nullptr);

	// Load all buttons from the builder
	bool all_btns_valid = true;
	for(auto it = mButtons.begin(); it != mButtons.end(); ++it) {
		builder->get_widget(it->first, std::get<0>(it->second));
		
		if(!std::get<0>(it->second))
			all_btns_valid = false;
	}

	if(all_btns_valid) {
		try { // Attempt to fetch all the buttons' icons
			std::get<1>(mButtons.at("line")) 			= new Gtk::Image(Gdk::Pixbuf::create_from_file("resources/textures/images/icons/sketch/tools/line.png", 60, 60));
			std::get<1>(mButtons.at("rectangle")) 		= new Gtk::Image(Gdk::Pixbuf::create_from_file("resources/textures/images/icons/sketch/tools/rectangle.png", 60, 60));
			std::get<1>(mButtons.at("circle")) 			= new Gtk::Image(Gdk::Pixbuf::create_from_file("resources/textures/images/icons/sketch/tools/circle.png", 60, 60));
			std::get<1>(mButtons.at("polygon")) 		= new Gtk::Image(Gdk::Pixbuf::create_from_file("resources/textures/images/icons/sketch/tools/polygon.png", 60, 60));
			std::get<1>(mButtons.at("threePointsArc")) 	= new Gtk::Image(Gdk::Pixbuf::create_from_file("resources/textures/images/icons/sketch/tools/threePointsArc.png", 60, 60));

			std::get<1>(mButtons.at("dimension")) 			= new Gtk::Image(Gdk::Pixbuf::create_from_file("resources/textures/images/icons/sketch/constraints/dimension.png", 30, 30));
			std::get<1>(mButtons.at("verticality")) 		= new Gtk::Image(Gdk::Pixbuf::create_from_file("resources/textures/images/icons/sketch/constraints/verticality.png", 30, 30));
			std::get<1>(mButtons.at("horizontality")) 		= new Gtk::Image(Gdk::Pixbuf::create_from_file("resources/textures/images/icons/sketch/constraints/horizontality.png", 30, 30));
			std::get<1>(mButtons.at("perpendicularity")) 	= new Gtk::Image(Gdk::Pixbuf::create_from_file("resources/textures/images/icons/sketch/constraints/perpendicularity.png", 30, 30));	
			std::get<1>(mButtons.at("parallelism")) 		= new Gtk::Image(Gdk::Pixbuf::create_from_file("resources/textures/images/icons/sketch/constraints/parallelism.png", 30, 30));
			std::get<1>(mButtons.at("coincidence")) 		= new Gtk::Image(Gdk::Pixbuf::create_from_file("resources/textures/images/icons/sketch/constraints/coincidence.png", 30, 30));
			std::get<1>(mButtons.at("equality")) 			= new Gtk::Image(Gdk::Pixbuf::create_from_file("resources/textures/images/icons/sketch/constraints/equality.png", 30, 30));

			std::get<1>(mButtons.at("to_svg")) = new Gtk::Image(Gdk::Pixbuf::create_from_file("resources/textures/images/icons/sketch/to_svg.png", 60, 60));			

			std::get<1>(mButtons.at("sketchFinish")) = new Gtk::Image(Gdk::Pixbuf::create_from_file("resources/textures/images/icons/sketch/sketchFinish.png", 60, 60));			
		} catch(const Glib::FileError& ex) {
			LOG_WARNING("Glib file error: " + ex.what());
		} catch(const Gdk::PixbufError& ex) {
			LOG_WARNING("Gtk pixbuf error: " + ex.what());
		}

		// Set all the buttons' icons		
		std::get<0>(mButtons.at("line"))->			set_image(*std::get<1>(mButtons.at("line")));
		std::get<0>(mButtons.at("rectangle"))->		set_image(*std::get<1>(mButtons.at("rectangle")));
		std::get<0>(mButtons.at("circle"))->		set_image(*std::get<1>(mButtons.at("circle")));
		std::get<0>(mButtons.at("polygon"))->		set_image(*std::get<1>(mButtons.at("polygon")));
		std::get<0>(mButtons.at("threePointsArc"))->set_image(*std::get<1>(mButtons.at("threePointsArc")));

		std::get<0>(mButtons.at("dimension"))->			set_image(*std::get<1>(mButtons.at("dimension")));
		std::get<0>(mButtons.at("verticality"))->		set_image(*std::get<1>(mButtons.at("verticality")));
		std::get<0>(mButtons.at("horizontality"))->		set_image(*std::get<1>(mButtons.at("horizontality")));
		std::get<0>(mButtons.at("perpendicularity"))->	set_image(*std::get<1>(mButtons.at("perpendicularity")));
		std::get<0>(mButtons.at("parallelism"))->		set_image(*std::get<1>(mButtons.at("parallelism")));
		std::get<0>(mButtons.at("coincidence"))->		set_image(*std::get<1>(mButtons.at("coincidence")));
		std::get<0>(mButtons.at("equality"))->			set_image(*std::get<1>(mButtons.at("equality")));

		std::get<0>(mButtons.at("to_svg"))->set_image(*std::get<1>(mButtons.at("to_svg")));

		std::get<0>(mButtons.at("sketchFinish"))->set_image(*std::get<1>(mButtons.at("sketchFinish")));

		
		// Set all the buttons' callback
		std::get<0>(mButtons.at("line"))->			signal_clicked().connect(sigc::mem_fun(*this, &sketchDesign::line));
		std::get<0>(mButtons.at("rectangle"))->		signal_clicked().connect(sigc::mem_fun(*this, &sketchDesign::rectangle));
		std::get<0>(mButtons.at("circle"))->		signal_clicked().connect(sigc::mem_fun(*this, &sketchDesign::circle));
		std::get<0>(mButtons.at("polygon"))->		signal_clicked().connect(sigc::mem_fun(*this, &sketchDesign::polygon));
		std::get<0>(mButtons.at("threePointsArc"))->signal_clicked().connect(sigc::mem_fun(*this, &sketchDesign::threePointsArc));

		std::get<0>(mButtons.at("dimension"))->			signal_clicked().connect(sigc::mem_fun(*this, &sketchDesign::dimension));
		std::get<0>(mButtons.at("verticality"))->		signal_clicked().connect(sigc::mem_fun(*this, &sketchDesign::verticality));
		std::get<0>(mButtons.at("perpendicularity"))->	signal_clicked().connect(sigc::mem_fun(*this, &sketchDesign::perpendicularity));
		std::get<0>(mButtons.at("parallelism"))->		signal_clicked().connect(sigc::mem_fun(*this, &sketchDesign::parallelism));
		std::get<0>(mButtons.at("coincidence"))->		signal_clicked().connect(sigc::mem_fun(*this, &sketchDesign::coincidence));
		std::get<0>(mButtons.at("equality"))->			signal_clicked().connect(sigc::mem_fun(*this, &sketchDesign::equality));

		std::get<0>(mButtons.at("to_svg"))->signal_clicked().connect(sigc::mem_fun(*this, &sketchDesign::to_svg));

		std::get<0>(mButtons.at("sketchFinish"))->signal_clicked().connect(sigc::mem_fun(*this, &sketchDesign::finish));

	} else {
		LOG_ERROR("Could not build sketch workspace.");
	}
}

void sketchDesign::line()
{
	set_tool("line");
}
void sketchDesign::rectangle()
{
//	mParentBloop->set_tool("rectangle");
}
void sketchDesign::polygon()
{
//	mParentBloop->set_tool("polygon");
}
void sketchDesign::circle()
{
//	mParentBloop->set_tool("circle");
}
void sketchDesign::threePointsArc()
{
//	mParentBloop->set_tool("threePointsArc");
}

void sketchDesign::dimension()
{
//	mParentBloop->set_tool("dimension");
}
void sketchDesign::verticality()
{
//	mParentBloop->set_tool("verticality");
}
void sketchDesign::horizontality()
{
//	mParentBloop->set_tool("horizontality");
}
void sketchDesign::perpendicularity()
{
//	mParentBloop->set_tool("perpendicularity");
}
void sketchDesign::parallelism()
{
//	mParentBloop->set_tool("parallelism");
}
void sketchDesign::coincidence()
{
//	mParentBloop->set_tool("coincidence");
}
void sketchDesign::equality()
{
//	mParentBloop->set_tool("equality");
}

void sketchDesign::to_svg()
{
	XML_document svgDoc;
	XML_declaration* dec = new XML_declaration;
	dec->add_lastAttribute(new XML_attribute("version", "1.0"));
	dec->add_lastAttribute(new XML_attribute("encoding", "utf-8"));
	svgDoc.add_lastChild(dec);
	XML_element* svg = new XML_element("svg");
	svgDoc.add_lastChild(svg);
	glm::vec2 min(std::numeric_limits<float>::max(), std::numeric_limits<float>::max()), max(std::numeric_limits<float>::min(), std::numeric_limits<float>::min());
	if(mState && mState->target) {
		mState->target->for_each([&svg, &min, &max, this](std::shared_ptr<entity> ent) {
			std::shared_ptr<svgEntity> svgEnt = std::dynamic_pointer_cast<svgEntity>(ent);
			std::shared_ptr<sketch> sk = std::dynamic_pointer_cast<sketch>(mState->target);
			if(svgEnt && sk) {
				svg->add_lastChild(svgEnt->to_svg(sk->basePlane().get(), min, max));
			}
		});
	}
	svg->set_attribute("width", "800");
	svg->set_attribute("height", "600");
	svg->set_attribute("viewbox", std::to_string(min.x) + " " + std::to_string(min.y) + " " + std::to_string(max.x-min.x) + " " + std::to_string(max.y-min.y));
	std::cout<<"Saving sketch in output.svg!\n";
	svgDoc.save("output.svg");
}

void sketchDesign::finish()
{
	mState->doc->push_action(std::shared_ptr<action>(new switchWorkspace_action(mState->doc, "partDesign"))); // Go to part design
	std::shared_ptr<workspaceState> newState = mState->doc->currentWorkspaceState(); // Find the new part design camera

	// // Record the part design camera's state
	camState targetState = newState->cam->state();
	newState->cam->set(mState->cam);
	mState->doc->push_action(std::shared_ptr<action>(new moveCamera_action(newState->cam, targetState, preferences::get_instance().get_long("camtrans")))); // Go to part design
}