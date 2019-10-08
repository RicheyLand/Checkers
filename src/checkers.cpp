#include "checkers.h"

Checkers::Checkers() : width(800), height(800)
{
	set_title("Checkers");
	set_default_size(width, height);
	set_border_width(0);
	override_background_color(Gdk::RGBA("black"), Gtk::STATE_FLAG_NORMAL);

	add(myBox);

	// myButton.set_label("Click");
	// myButton.signal_clicked().connect(sigc::bind<Glib::ustring>(sigc::mem_fun(*this, &Checkers::onButtonClicked), "Button 1"));
	// myGrid.add(myButton);

	add_events(Gdk::STRUCTURE_MASK);
	signal_configure_event().connect(sigc::mem_fun(*this, &Checkers::onConfigureChanged), false);

	for (int i = 0; i < blockCount; i++)
	{
		for (int j = 0; j < blockCount; j++)
		{
			string message = to_string(i) + to_string(j);
			string path = "";

			if (i & 1)
			{
				if (j & 1)
					path = "./resources/block_brown.jpg";
				else
					path = "./resources/block_white.jpg";
			}
			else
			{
				if (j & 1)
					path = "./resources/block_white.jpg";
				else
					path = "./resources/block_brown.jpg";
			}

			// Glib::RefPtr<Gdk::Pixbuf> image = Gdk::Pixbuf::create_from_file(path, 90, 90, true);
			Glib::RefPtr<Gdk::Pixbuf> image = Gdk::Pixbuf::create_from_file(path);
			images[i][j].set(image);

			// images[i][j].set_hexpand(true);
			// images[i][j].set_vexpand(true);
			// eventBoxes[i][j].set_halign(Gtk::ALIGN_FILL);
			// eventBoxes[i][j].set_valign(Gtk::ALIGN_FILL);
			eventBoxes[i][j].add(images[i][j]);
			eventBoxes[i][j].set_events(Gdk::BUTTON_PRESS_MASK);
			eventBoxes[i][j].signal_button_press_event().connect(sigc::bind<Glib::ustring>(sigc::mem_fun(*this, &Checkers::onEventboxButtonPress), message));
			eventBoxes[i][j].set_tooltip_text("Click to exit");

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

bool Checkers::onConfigureChanged(GdkEventConfigure * event)
{
	int newWidth = event->width;
	int newHeight = event->height;

	// cout << "Configure changed" << " " << event->width << endl;

	if (newWidth != width)
		width = newWidth;

	if (newHeight != height)
		height = newHeight;

	return false;
}
