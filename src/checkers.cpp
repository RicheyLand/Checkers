#include "checkers.h"

Checkers::Checkers()
{
	set_title("Checkers");
	set_default_size(640, 480);

	set_border_width(10);

	add(myBox);

	myButton.set_label("Click");
	myImage.set("./resources/block_brown.jpg");

	myButton.signal_clicked().connect(sigc::bind<Glib::ustring>(sigc::mem_fun(*this, &Checkers::onButtonClicked), "Button 1"));

	myBox.pack_start(myButton);

	myButton.show();

	m_EventBox.add(myImage);

	myBox.pack_start(m_EventBox);

	m_EventBox.set_events(Gdk::BUTTON_PRESS_MASK);
	m_EventBox.signal_button_press_event().connect(sigc::mem_fun(*this, &Checkers::onEventboxButtonPress));

	m_EventBox.set_tooltip_text("Click to exit");

	show_all_children();
}

Checkers::~Checkers()
{
}

void Checkers::onButtonClicked(Glib::ustring data)
{
	std::cout << data << " was pressed" << std::endl;
}

bool Checkers::onEventboxButtonPress(GdkEventButton*)
{
	hide();
	return true;
}
