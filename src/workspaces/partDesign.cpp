
#include "partDesign.hpp"
#include "../bloop.hpp"

partDesign::partDesign(bloop* parent) :
	workspace(parent)
{}

partDesign::partDesign(Glib::RefPtr<Gtk::Builder> const& builder, bloop* parent) :
	workspace("partDesign_upperBar", builder, parent),
	mStartSketch_btn(nullptr),
	mExtrude_btn(nullptr)
{
	mTools["startSketch"] 	= std::shared_ptr<tool_abstract>(new startSketch_tool(mState));
	mTools["extrusion"] 	= std::shared_ptr<tool_abstract>(new extrusion_tool(mState));

	mDefaultTool = mTools.at("simpleSelector");

	builder->get_widget("startSketch", mStartSketch_btn);
	builder->get_widget("extrusion", mExtrude_btn);

	if(mStartSketch_btn && mExtrude_btn) {
		try {
			auto pixbuf = Gdk::Pixbuf::create_from_file("resources/textures/images/icons/partDesign/startSketch.png", 60, 60);
			guint32 color = 2000;
			pixbuf->composite_color_simple(60, 60, Gdk::InterpType::INTERP_NEAREST, 255, 50, color, color);
			mStartSketch_icon = new Gtk::Image(pixbuf);
			mStartSketch_icon->set_tooltip_text("Start sketch");

			mExtrude_icon = new Gtk::Image(Gdk::Pixbuf::create_from_file("resources/textures/images/icons/partDesign/extrusion.png", 60, 60));
			mExtrude_icon->set_tooltip_text("Extrude");
		} catch(const Glib::FileError& ex) {
			LOG_ERROR("Glib file error: " + ex.what());
		} catch(const Gdk::PixbufError& ex) {
			LOG_ERROR("Gtk pixbuf error: " + ex.what());
		}
		
		mStartSketch_btn->set_image(*mStartSketch_icon);
		mStartSketch_btn->signal_clicked().connect(sigc::mem_fun(*this, &partDesign::startSketch));
		mExtrude_btn->set_image(*mExtrude_icon);
		mExtrude_btn->signal_clicked().connect(sigc::mem_fun(*this, &partDesign::extrusion));
	} else {
		LOG_ERROR("Could not build part design workspace.");
	}
}

void partDesign::startSketch()
{
	set_tool_target<part>("startSketch");
	// mParentBloop->currentDocument()->set_tool(mTools.at("startSketch"));
	// mParentBloop->set_tool("startSketch");
}
void partDesign::extrusion()
{
	set_tool_target<part>("extrusion");
	// mParentBloop->currentDocument()->set_tool(mTools.at("extrusion"));
	// mParentBloop->set_tool("extrusion");
}