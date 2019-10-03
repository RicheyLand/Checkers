#ifndef CHECKERS_H
#define CHECKERS_H

#include <gtkmm.h>

class Checkers : public Gtk::Window
{
public:
	Checkers();
	virtual ~Checkers();

protected:
	void handleButtonClick();

	Gtk::Button myButton;
};

#endif
