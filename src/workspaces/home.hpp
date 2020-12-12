
#ifndef HOME_HPP_
#define HOME_HPP_

#include "workspace.hpp"

#include <map>

/*
	@class home describes the home workspace, it will eventually be the place to 
	create new files, open files and have previews, but now it is an image and buttons that do nothing
	@parent : workspace
*/
class home : public workspace {
private:
	std::map<std::string, std::pair<Gtk::Button*, Gtk::Image*>> mButtons; // Buttons with images and names
public:
	/*
		@function home creates the home workspace

		@param parent : The window which it is part of

		@note : Not recommended as it doesn't create buttons or link callbacks
	*/
	home(bloop* parent);
	/*
		@function home creates the home workspace with a xml file

		@param builder : 	The gtk builder that creates widget from an xml file
		@param parent : 	The window which it is part of
	*/
	home(Glib::RefPtr<Gtk::Builder> const& builder, bloop* parent);

	/*
		@function new_file will eventually create a file
	*/
	void new_file();
	/*
		@function open_file will eventually open an openfile dialog of some sort
	*/
	void open_file();
};

#endif