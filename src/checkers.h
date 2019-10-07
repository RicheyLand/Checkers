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

	void onButtonClicked(Glib::ustring data);
	bool onEventboxButtonPress(GdkEventButton* button_event);

	Gtk::EventBox m_EventBox;

	Gtk::Box myBox;
	Gtk::Button myButton;
	Gtk::Image myImage;
};

#endif
