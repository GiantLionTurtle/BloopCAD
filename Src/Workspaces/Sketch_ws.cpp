
// BloopCAD
// Copyright (C) 2020  BloopCorp

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "Sketch_ws.hpp"

#include <Utils/Expunge.hpp>
#include <Utils/Preferences.hpp>
#include <Utils/Maths_util.hpp>
#include <Actions/Part/EnterPart_action.hpp>
#include <Actions/Common/MoveCamera_action.hpp>
#include <Actions/Common/Serial_action.hpp>
#include <Tools/Sketch/Line_tool.hpp>
#include <Tools/Sketch/Point_tool.hpp>
#include <Tools/Sketch/Circle_tool.hpp>
#include <Tools/Sketch/Coincidence_tool.hpp>
#include <Tools/Sketch/Perpendicularity_tool.hpp>
#include <Tools/Sketch/Verticality_tool.hpp>
#include <Tools/Sketch/Horizontality_tool.hpp>
#include <Tools/Sketch/SketchDefault_tool.hpp>
#include <Tools/Sketch/Pan2d_tool.hpp>
#include <Tools/Sketch/Zoom2d_tool.hpp>
#include <Utils/XMLParser.hpp>
#include <Bloop/Bloop.hpp>
#include "Document.hpp"

Sketch_ws::Sketch_ws(Glib::RefPtr<Gtk::Builder> const& builder, Bloop* parent) :
	Workspace_abstr("Sketch_ws_upperBar", builder, parent) // Create base workspace with upper bar
{	
	// Create all the tools used in this workspace
	mPan3d_tool 					= new Pan2d_tool(this);
	mZoom3d_tool					= new Zoom2d_tool(this);
	mSketchDefault_tool 			= new SketchDefault_tool(this);
	mPoint_tool 					= new Point_tool(this);
	mLine_tool 						= new Line_tool(this);
	mCircle_tool 					= new Circle_tool(this);
	mCoincidence_tool 				= new Coincidence_tool(this);
	mVerticality_tool 				= new Verticality_tool(this);
	mHorizontality_tool 			= new Horizontality_tool(this);
	mPerpendiculatiry_tool 			= new Perpendicularity_tool(this);
	mDefaultTool = mSketchDefault_tool;

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
			std::get<1>(mButtons.at("line")) 			= new Gtk::Image(Gdk::Pixbuf::create_from_file("Resources/Textures/Images/Icons/Sketch/Tools/Line.png", 60, 60));
			std::get<1>(mButtons.at("point")) 			= new Gtk::Image(Gdk::Pixbuf::create_from_file("Resources/Textures/Images/Icons/Sketch/Tools/Point.png", 60, 60));
			std::get<1>(mButtons.at("rectangle")) 		= new Gtk::Image(Gdk::Pixbuf::create_from_file("Resources/Textures/Images/Icons/Sketch/Tools/Rectangle.png", 60, 60));
			std::get<1>(mButtons.at("circle")) 			= new Gtk::Image(Gdk::Pixbuf::create_from_file("Resources/Textures/Images/Icons/Sketch/Tools/Circle.png", 60, 60));
			std::get<1>(mButtons.at("polygon")) 		= new Gtk::Image(Gdk::Pixbuf::create_from_file("Resources/Textures/Images/Icons/Sketch/Tools/Polygon.png", 60, 60));
			std::get<1>(mButtons.at("threePointsArc")) 	= new Gtk::Image(Gdk::Pixbuf::create_from_file("Resources/Textures/Images/Icons/Sketch/Tools/ThreePointsArc.png", 60, 60));

			std::get<1>(mButtons.at("dimension")) 			= new Gtk::Image(Gdk::Pixbuf::create_from_file("Resources/Textures/Images/Icons/Sketch/Constraints/Dimension.png", 30, 30));
			std::get<1>(mButtons.at("verticality")) 		= new Gtk::Image(Gdk::Pixbuf::create_from_file("Resources/Textures/Images/Icons/Sketch/Constraints/Verticality.png", 30, 30));
			std::get<1>(mButtons.at("horizontality")) 		= new Gtk::Image(Gdk::Pixbuf::create_from_file("Resources/Textures/Images/Icons/Sketch/Constraints/Horizontality.png", 30, 30));
			std::get<1>(mButtons.at("perpendicularity")) 	= new Gtk::Image(Gdk::Pixbuf::create_from_file("Resources/Textures/Images/Icons/Sketch/Constraints/Perpendicularity.png", 30, 30));	
			std::get<1>(mButtons.at("parallelism")) 		= new Gtk::Image(Gdk::Pixbuf::create_from_file("Resources/Textures/Images/Icons/Sketch/Constraints/Parallelism.png", 30, 30));
			std::get<1>(mButtons.at("coincidence")) 		= new Gtk::Image(Gdk::Pixbuf::create_from_file("Resources/Textures/Images/Icons/Sketch/Constraints/Coincidence.png", 30, 30));
			std::get<1>(mButtons.at("equality")) 			= new Gtk::Image(Gdk::Pixbuf::create_from_file("Resources/Textures/Images/Icons/Sketch/Constraints/Equality.png", 30, 30));

			std::get<1>(mButtons.at("to_svg")) = new Gtk::Image(Gdk::Pixbuf::create_from_file("Resources/Textures/Images/Icons/Sketch/To_svg.png", 60, 60));			

			std::get<1>(mButtons.at("sketchFinish")) = new Gtk::Image(Gdk::Pixbuf::create_from_file("Resources/Textures/Images/Icons/Sketch/SketchFinish.png", 60, 60));			
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
		std::get<0>(mButtons.at("line"))->			signal_clicked().connect(sigc::mem_fun(*this, &Sketch_ws::begin_line));
		std::get<0>(mButtons.at("point"))->			signal_clicked().connect(sigc::mem_fun(*this, &Sketch_ws::begin_point));
		std::get<0>(mButtons.at("rectangle"))->		signal_clicked().connect(sigc::mem_fun(*this, &Sketch_ws::begin_rectangle));
		std::get<0>(mButtons.at("circle"))->		signal_clicked().connect(sigc::mem_fun(*this, &Sketch_ws::begin_circle));
		std::get<0>(mButtons.at("polygon"))->		signal_clicked().connect(sigc::mem_fun(*this, &Sketch_ws::begin_polygon));
		std::get<0>(mButtons.at("threePointsArc"))->signal_clicked().connect(sigc::mem_fun(*this, &Sketch_ws::begin_threePointsArc));

		std::get<0>(mButtons.at("dimension"))->			signal_clicked().connect(sigc::mem_fun(*this, &Sketch_ws::begin_dimension));
		std::get<0>(mButtons.at("verticality"))->		signal_clicked().connect(sigc::mem_fun(*this, &Sketch_ws::begin_verticality));
		std::get<0>(mButtons.at("horizontality"))->		signal_clicked().connect(sigc::mem_fun(*this, &Sketch_ws::begin_horizontality));
		std::get<0>(mButtons.at("perpendicularity"))->	signal_clicked().connect(sigc::mem_fun(*this, &Sketch_ws::begin_perpendicularity));
		std::get<0>(mButtons.at("parallelism"))->		signal_clicked().connect(sigc::mem_fun(*this, &Sketch_ws::begin_parallelism));
		std::get<0>(mButtons.at("coincidence"))->		signal_clicked().connect(sigc::mem_fun(*this, &Sketch_ws::begin_coincidence));
		std::get<0>(mButtons.at("equality"))->			signal_clicked().connect(sigc::mem_fun(*this, &Sketch_ws::begin_equality));

		std::get<0>(mButtons.at("verticality"))->		set_tooltip_text("verticality");
		std::get<0>(mButtons.at("horizontality"))->		set_tooltip_text("horizontality");
		std::get<0>(mButtons.at("perpendicularity"))->	set_tooltip_text("perpendicularity");
		std::get<0>(mButtons.at("parallelism"))->		set_tooltip_text("parallelism");
		std::get<0>(mButtons.at("coincidence"))->		set_tooltip_text("coincidence");
		std::get<0>(mButtons.at("equality"))->			set_tooltip_text("equality");

		std::get<0>(mButtons.at("to_svg"))->signal_clicked().connect(sigc::mem_fun(*this, &Sketch_ws::to_svg));

		std::get<0>(mButtons.at("sketchFinish"))->signal_clicked().connect(sigc::mem_fun(*this, &Sketch_ws::finish));

	} else {
		LOG_ERROR("Could not build sketch Workspace_abstr.");
	}	
}
Sketch_ws::~Sketch_ws()
{
	expunge(mPan3d_tool);
	expunge(mZoom3d_tool);
	expunge(mSketchDefault_tool);
	expunge(mPoint_tool);
	expunge(mLine_tool);
	expunge(mCircle_tool);
	expunge(mCoincidence_tool);
	expunge(mVerticality_tool);
	expunge(mHorizontality_tool);
	expunge(mPerpendiculatiry_tool);
	mDefaultTool = nullptr;

	for(auto it = mButtons.begin(); it != mButtons.end(); ++it) {
		expunge(it->second.first);
		expunge(it->second.second);
	}
}

bool Sketch_ws::set_tool(int name)
{
	Tool_abstract* to_set = nullptr;
	switch(name) {
	case TOOLIDS::TOOLID_SKETCHDESIGNDEFAULT:
		to_set = mSketchDefault_tool;
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
		to_set = mPan3d_tool;
		break;
	case TOOLIDS::TOOLID_ZOOM:
		to_set = mZoom3d_tool;
		break;
	default:
		return Workspace_abstr::set_tool(name);
	}
	Workspace_abstr::set_tool(to_set);
	return true;
}

bool Sketch_ws::manage_key_press(GdkEventKey* event)
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
		return Workspace_abstr::manage_key_press(event);
	}
	return true;
}
bool Sketch_ws::manage_mouse_scroll(GdkEventScroll* event)
{
	return mZoom3d_tool->manage_scroll(event);
}
void Sketch_ws::begin_line()
{
	set_tool(TOOLIDS::TOOLID_LINE);
}
void Sketch_ws::begin_point()
{
	set_tool(TOOLIDS::TOOLID_POINT);
}
void Sketch_ws::begin_rectangle()
{
	LOG_WARNING("This tool is not available yet.");
}
void Sketch_ws::begin_polygon()
{
	LOG_WARNING("This tool is not available yet.");
}
void Sketch_ws::begin_circle()
{
	set_tool(TOOLIDS::TOOLID_CIRCLE);
}
void Sketch_ws::begin_threePointsArc()
{
	LOG_WARNING("This tool is not available yet.");
}

void Sketch_ws::begin_dimension()
{
	LOG_WARNING("This tool is not available yet.");
}
void Sketch_ws::begin_verticality()
{
	set_tool(TOOLIDS::TOOLID_VERTICALITY);
}
void Sketch_ws::begin_horizontality()
{
	set_tool(TOOLIDS::TOOLID_HORIZONTALITY);
}
void Sketch_ws::begin_perpendicularity()
{
	set_tool(TOOLIDS::TOOLID_PERPENDICULARITY);
}
void Sketch_ws::begin_parallelism()
{
	LOG_WARNING("This tool is not available yet.");
}
void Sketch_ws::begin_coincidence()
{
	set_tool(TOOLIDS::TOOLID_COINCIDENCE);
}
void Sketch_ws::begin_equality()
{
	LOG_WARNING("This tool is not available yet.");
}

void Sketch_ws::to_svg()
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
}

void Sketch_ws::finish()
{
	mState->doc->push_action(Action_ptr(new Serial_action({
		Action_ptr(new EnterPart_action(true)),
		Action_ptr(new MoveCamera_action(nullptr, mState->startCamState, Preferences::get_instance().get_long("camtrans")))
	})));
}