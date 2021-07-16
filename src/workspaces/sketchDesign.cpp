
#include "sketchDesign.hpp"

#include <utils/preferences.hpp>
#include <utils/mathUtils.hpp>
#include <actions/partDesign/enterPartDesign_action.hpp>
#include <actions/sketchDesign/quitSketchDesign_action.hpp>
#include <actions/common/moveCamera_action.hpp>
#include <actions/common/serial_action.hpp>
#include <tools/sketchDesign/line_tool.hpp>
#include <tools/sketchDesign/point_tool.hpp>
#include <tools/sketchDesign/circle_tool.hpp>
#include <tools/sketchDesign/coincidence_tool.hpp>
#include <tools/sketchDesign/perpendicularity_tool.hpp>
#include <tools/sketchDesign/verticality_tool.hpp>
#include <tools/sketchDesign/horizontality_tool.hpp>
#include <tools/sketchDesign/sketchDesignDefault_tool.hpp>
#include <tools/sketchDesign/pan2d_tool.hpp>
#include <tools/sketchDesign/zoom2d_tool.hpp>
#include <utils/xmlParser.hpp>
#include <Drawables/svgEntity.hpp>
#include <bloop.hpp>

sketchDesign::sketchDesign(bloop* parent) :
	workspace(parent)
{
	
}

sketchDesign::sketchDesign(Glib::RefPtr<Gtk::Builder> const& builder, bloop* parent) :
	workspace("sketchDesign_upperBar", builder, parent) // Create base workspace with upper bar
{	
	// Create all the tools used in this workspace
	mPan_tool 					= std::make_shared<pan2d_tool>(this);
	mZoom_tool					= std::make_shared<zoom2d_tool>(this);
	mSketchDesignDefault_tool 	= std::make_shared<sketchDesignDefault_tool>(this);
	mPoint_tool 				= std::make_shared<point_tool>(this);
	mLine_tool 					= std::make_shared<line_tool>(this);
	mCircle_tool 				= std::make_shared<circle_tool>(this);
	mCoincidence_tool 			= std::make_shared<coincidence_tool>(this);
	mVerticality_tool 			= std::make_shared<verticality_tool>(this);
	mHorizontality_tool 		= std::make_shared<horizontality_tool>(this);
	mPerpendiculatiry_tool 		= std::make_shared<perpendicularity_tool>(this);
	mDefaultTool = mSketchDesignDefault_tool;

	// Initialize all buttons as 2 nullptr
	mButtons["line"] 			= std::make_pair<Gtk::Button*, Gtk::Image*>(nullptr, nullptr);
	mButtons["point"] 			= std::make_pair<Gtk::Button*, Gtk::Image*>(nullptr, nullptr);
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
			std::get<1>(mButtons.at("point")) 			= new Gtk::Image(Gdk::Pixbuf::create_from_file("resources/textures/images/icons/sketch/tools/point.png", 60, 60));
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
		std::get<0>(mButtons.at("point"))->			set_image(*std::get<1>(mButtons.at("point")));
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
		std::get<0>(mButtons.at("line"))->			signal_clicked().connect(sigc::mem_fun(*this, &sketchDesign::begin_line));
		std::get<0>(mButtons.at("point"))->			signal_clicked().connect(sigc::mem_fun(*this, &sketchDesign::begin_point));
		std::get<0>(mButtons.at("rectangle"))->		signal_clicked().connect(sigc::mem_fun(*this, &sketchDesign::begin_rectangle));
		std::get<0>(mButtons.at("circle"))->		signal_clicked().connect(sigc::mem_fun(*this, &sketchDesign::begin_circle));
		std::get<0>(mButtons.at("polygon"))->		signal_clicked().connect(sigc::mem_fun(*this, &sketchDesign::begin_polygon));
		std::get<0>(mButtons.at("threePointsArc"))->signal_clicked().connect(sigc::mem_fun(*this, &sketchDesign::begin_threePointsArc));

		std::get<0>(mButtons.at("dimension"))->			signal_clicked().connect(sigc::mem_fun(*this, &sketchDesign::begin_dimension));
		std::get<0>(mButtons.at("verticality"))->		signal_clicked().connect(sigc::mem_fun(*this, &sketchDesign::begin_verticality));
		std::get<0>(mButtons.at("horizontality"))->		signal_clicked().connect(sigc::mem_fun(*this, &sketchDesign::begin_horizontality));
		std::get<0>(mButtons.at("perpendicularity"))->	signal_clicked().connect(sigc::mem_fun(*this, &sketchDesign::begin_perpendicularity));
		std::get<0>(mButtons.at("parallelism"))->		signal_clicked().connect(sigc::mem_fun(*this, &sketchDesign::begin_parallelism));
		std::get<0>(mButtons.at("coincidence"))->		signal_clicked().connect(sigc::mem_fun(*this, &sketchDesign::begin_coincidence));
		std::get<0>(mButtons.at("equality"))->			signal_clicked().connect(sigc::mem_fun(*this, &sketchDesign::begin_equality));

		std::get<0>(mButtons.at("verticality"))->		set_tooltip_text("verticality");
		std::get<0>(mButtons.at("horizontality"))->		set_tooltip_text("horizontality");
		std::get<0>(mButtons.at("perpendicularity"))->	set_tooltip_text("perpendicularity");
		std::get<0>(mButtons.at("parallelism"))->		set_tooltip_text("parallelism");
		std::get<0>(mButtons.at("coincidence"))->		set_tooltip_text("coincidence");
		std::get<0>(mButtons.at("equality"))->			set_tooltip_text("equality");

		std::get<0>(mButtons.at("to_svg"))->signal_clicked().connect(sigc::mem_fun(*this, &sketchDesign::to_svg));

		std::get<0>(mButtons.at("sketchFinish"))->signal_clicked().connect(sigc::mem_fun(*this, &sketchDesign::finish));

	} else {
		LOG_ERROR("Could not build sketch workspace.");
	}	
}

bool sketchDesign::set_tool(int name)
{
	tool_abstract_ptr to_set = nullptr;
	switch(name) {
	case TOOLIDS::TOOLID_SKETCHDESIGNDEFAULT:
		to_set = mSketchDesignDefault_tool;
		break;
	case TOOLIDS::TOOLID_POINT:
		to_set = mPoint_tool;
		break;
	case TOOLIDS::TOOLID_LINE:
		to_set = mLine_tool;
		break;
	case TOOLIDS::TOOLID_CIRCLE:
		to_set = mCircle_tool;
		break;
	case TOOLIDS::TOOLID_COINCIDENCE:
		to_set = mCoincidence_tool;
		break;
	case TOOLIDS::TOOLID_VERTICALITY:
		to_set = mVerticality_tool;
		break;
	case TOOLIDS::TOOLID_HORIZONTALITY:
		to_set = mHorizontality_tool;
		break;
	case TOOLIDS::TOOLID_PERPENDICULARITY:
		to_set = mPerpendiculatiry_tool;
		break;
	case TOOLIDS::TOOLID_PAN:
		to_set = mPan_tool;
		break;
	case TOOLIDS::TOOLID_ZOOM:
		to_set = mZoom_tool;
		break;
	default:
		return workspace::set_tool(name);
	}
	workspace::set_tool(to_set);
	return true;
}

bool sketchDesign::manage_key_press(GdkEventKey* event)
{
	switch(event->keyval) {
	case GDK_KEY_L:
	case GDK_KEY_l:
		begin_line();
		break;
	case GDK_KEY_C:
	case GDK_KEY_c:
		begin_coincidence();
		break;
	default:
		return workspace::manage_key_press(event);
	}
	return true;
}
bool sketchDesign::manage_mouse_scroll(GdkEventScroll* event)
{
	return mZoom_tool->manage_scroll(event);
}
void sketchDesign::begin_line()
{
	set_tool(TOOLIDS::TOOLID_LINE);
}
void sketchDesign::begin_point()
{
	set_tool(TOOLIDS::TOOLID_POINT);
}
void sketchDesign::begin_rectangle()
{
	LOG_WARNING("This tool is not available yet.");
}
void sketchDesign::begin_polygon()
{
	LOG_WARNING("This tool is not available yet.");
}
void sketchDesign::begin_circle()
{
	set_tool(TOOLIDS::TOOLID_CIRCLE);
}
void sketchDesign::begin_threePointsArc()
{
	LOG_WARNING("This tool is not available yet.");
}

void sketchDesign::begin_dimension()
{
	LOG_WARNING("This tool is not available yet.");
}
void sketchDesign::begin_verticality()
{
	set_tool(TOOLIDS::TOOLID_VERTICALITY);
}
void sketchDesign::begin_horizontality()
{
	set_tool(TOOLIDS::TOOLID_HORIZONTALITY);
}
void sketchDesign::begin_perpendicularity()
{
	set_tool(TOOLIDS::TOOLID_PERPENDICULARITY);
}
void sketchDesign::begin_parallelism()
{
	LOG_WARNING("This tool is not available yet.");
}
void sketchDesign::begin_coincidence()
{
	set_tool(TOOLIDS::TOOLID_COINCIDENCE);
}
void sketchDesign::begin_equality()
{
	LOG_WARNING("This tool is not available yet.");
}

void sketchDesign::to_svg()
{
	// XML_document svgDoc;
	// XML_declaration* dec = new XML_declaration;
	// dec->add_lastAttribute(new XML_attribute("version", "1.0"));
	// dec->add_lastAttribute(new XML_attribute("encoding", "utf-8"));
	// svgDoc.add_lastChild(dec);
	// XML_element* svg = new XML_element("svg");
	// svgDoc.add_lastChild(svg);
	// glm::vec2 min(std::numeric_limits<float>::max(), std::numeric_limits<float>::max()), max(std::numeric_limits<float>::min(), std::numeric_limits<float>::min());
	// if(mState && mState->target) {
	// 	mState->target->for_each([&svg, &min, &max, this](Drawable* ent) {
	// 		std::shared_ptr<svgEntity> svgEnt = std::dynamic_pointer_cast<svgEntity>(ent);
	// 		Sketch* sk = target();
	// 		if(svgEnt && sk) {
	// 			svg->add_lastChild(svgEnt->to_svg(sk->basePlane().get(), min, max));
	// 		}
	// 	});
	// }
	// svg->set_attribute("width", "800");
	// svg->set_attribute("height", "600");
	// svg->set_attribute("viewbox", std::to_string(min.x) + " " + std::to_string(min.y) + " " + std::to_string(max.x-min.x) + " " + std::to_string(max.y-min.y));
	// std::cout<<"Saving sketch in output.svg!\n";
	// svgDoc.save("output.svg");
	target()->print();
}

void sketchDesign::finish()
{
	mState->doc->push_action(std::shared_ptr<action>(new serial_action({
		std::shared_ptr<action>(new quitSketchDesign_action(target())),
		std::shared_ptr<action>(new enterPartDesign_action(true)),
		std::shared_ptr<action>(new moveCamera_action(nullptr, mState->startCamState, preferences::get_instance().get_long("camtrans")))
	})));
}