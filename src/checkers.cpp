#include "checkers.h"
#include <iostream>

Checkers::Checkers() : myButton("Click")
{
	set_title("Checkers");
	set_border_width(10);
	set_default_size(640, 480);

	myButton.signal_clicked().connect(sigc::mem_fun(*this, &Checkers::handleButtonClick));
	add(myButton);
	myButton.show();
}

Checkers::~Checkers()
{
}

void Checkers::handleButtonClick()
{
	std::cout << "Button clicked" << std::endl;
}
