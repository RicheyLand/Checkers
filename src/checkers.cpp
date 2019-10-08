#include "checkers.h"

Checkers::Checkers()
{
	set_title("Checkers");
	set_default_size(800, 800);
	set_border_width(3);
	override_background_color(Gdk::RGBA("black"), Gtk::STATE_FLAG_NORMAL);

	add(myBox);

	// myButton.set_label("Click");
	// myButton.signal_clicked().connect(sigc::bind<Glib::ustring>(sigc::mem_fun(*this, &Checkers::onButtonClicked), "Button 1"));
	// myGrid.add(myButton);

	for (int i = 0; i < blockCount; i++)
	{
		for (int j = 0; j < blockCount; j++)
		{
			string message;
			message = to_string(i) + to_string(j);

			if (i & 1)
			{
				if (j & 1)
					images[i][j].set("./resources/block_brown.jpg");
				else
					images[i][j].set("./resources/block_white.jpg");
			}
			else
			{
				if (j & 1)
					images[i][j].set("./resources/block_white.jpg");
				else
					images[i][j].set("./resources/block_brown.jpg");
			}

			eventBoxes[i][j].add(images[i][j]);
			eventBoxes[i][j].set_events(Gdk::BUTTON_PRESS_MASK);
			eventBoxes[i][j].signal_button_press_event().connect(sigc::bind<Glib::ustring>(sigc::mem_fun(*this, &Checkers::onEventboxButtonPress), message));
			eventBoxes[i][j].set_tooltip_text("Click to exit");

			eventBoxes[i][j].set_hexpand(true);
			eventBoxes[i][j].set_halign(Gtk::ALIGN_FILL);
			eventBoxes[i][j].set_vexpand(true);
			eventBoxes[i][j].set_valign(Gtk::ALIGN_FILL);


			myGrid.attach(eventBoxes[i][j], i, j, 1, 1);
		}
	}

	myBox.pack_start(myGrid);

	show_all_children();
}

Checkers::~Checkers()
{
}

void Checkers::onButtonClicked(Glib::ustring data)
{
	std::cout << data << " was pressed" << std::endl;
}

bool Checkers::onEventboxButtonPress(GdkEventButton * /*button_event*/, Glib::ustring data)
{
	cout << data << endl;

	// hide();
	return true;
}
