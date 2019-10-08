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

private:
	void onButtonClicked(Glib::ustring);
	bool onEventboxButtonPress(GdkEventButton *, Glib::ustring);
	bool onConfigureChanged(GdkEventConfigure *);

	static const int blockCount = 8;

	int width;
	int height;
	
	Gtk::Box myBox;
	Gtk::Grid myGrid;
	Gtk::Button myButton;

	Gtk::Image images[blockCount][blockCount];
	Gtk::EventBox eventBoxes[blockCount][blockCount];
};

#endif
