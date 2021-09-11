
#ifndef MISCUI_HPP_
#define MISCUI_HPP_

#include <string>

#include <gtkmm.h>

/*
	@class TabButton describes a tab with a label and a close button for the Document indexer
	@parent : Gtk::Box
*/
class TabButton : public Gtk::Box {
private:
	Gtk::Label mLabel; // The label set by the Document/the window
	Gtk::Button mCloseButton; // The close button, it is a 'x' character
public:
	/*
		@function TabButton creates a tab object

		@param label_ : The label on the tab
	*/
	TabButton(std::string const& label_);

	/*
		@function set_label sets the displayed label on the tab

		@param label_ : The label to be set
	*/
	void set_label(std::string const& label_) { mLabel.set_text(label_); }
	/*
		@function label 

		@return : The tab's text label
	*/
	std::string label() { return mLabel.get_text(); }
};

class StatusBar : public Gtk::Box {
public:
	enum status { WORKSPACE, TOOL };
private:
	Gtk::Label* workspaceName, * toolName;
public:
	StatusBar(BaseObjectType* cobject, Glib::RefPtr<Gtk::Builder> const& builder);

	void update(status which, std::string const& content);
};

#endif