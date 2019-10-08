#ifndef CHECKERS_H
#define CHECKERS_H

#include <gtkmm.h>
#include <iostream>
using namespace std;

class Checkers : public Gtk::Window
{
public:
	Checkers();
	virtual ~Checkers();

protected:

	void onButtonClicked(Glib::ustring);
	bool onEventboxButtonPress(GdkEventButton *, Glib::ustring);

	static const int blockCount = 8;
	
	Gtk::Box myBox;
	Gtk::Grid myGrid;
	Gtk::Button myButton;

	Gtk::Image images[blockCount][blockCount];
	Gtk::EventBox eventBoxes[blockCount][blockCount];
};

#endif
