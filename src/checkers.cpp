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

	brownImage = Gdk::Pixbuf::create_from_file("./resources/block_brown.jpg", 100, 100, true);
	whiteImage = Gdk::Pixbuf::create_from_file("./resources/block_white.jpg", 100, 100, true);

	for (int i = 0; i < blockCount; i++)
	{
		for (int j = 0; j < blockCount; j++)
		{
			string message = to_string(i) + to_string(j);

			if (i & 1)
			{
				if (j & 1)
					images[i][j].set(brownImage);
				else
					images[i][j].set(whiteImage);
			}
			else
			{
				if (j & 1)
					images[i][j].set(whiteImage);
				else
					images[i][j].set(brownImage);
			}

			// images[i][j].set_hexpand(true);
			// images[i][j].set_vexpand(true);
			// eventBoxes[i][j].set_halign(Gtk::ALIGN_FILL);
			// eventBoxes[i][j].set_valign(Gtk::ALIGN_FILL);
			eventBoxes[i][j].add(images[i][j]);
			eventBoxes[i][j].set_events(Gdk::BUTTON_PRESS_MASK);
			eventBoxes[i][j].signal_button_press_event().connect(sigc::bind<Glib::ustring>(sigc::mem_fun(*this, &Checkers::onEventboxButtonPress), message));
			// eventBoxes[i][j].set_tooltip_text("Click to exit");

			myGrid.attach(eventBoxes[i][j], i, j, 1, 1);
		}
	}

	scrolledWindow.add(myGrid);

	myBox.pack_start(scrolledWindow);

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

	bool changed = false;

	if (newWidth != width)
	{
		width = newWidth;
		changed = true;
	}

	if (newHeight != height)
	{
		height = newHeight;
		changed = true;
	}

	if (changed == false)
		return false;

	if (width < height)
		width = height;
	else if (width > height)
		height = width;
	
	int chunk = height / 8;
	int trail = height % 8;

	brownImage = Gdk::Pixbuf::create_from_file("./resources/block_brown.jpg", chunk, chunk, true);
	whiteImage = Gdk::Pixbuf::create_from_file("./resources/block_white.jpg", chunk, chunk, true);

	for (int i = 0; i < blockCount; i++)
	{
		for (int j = 0; j < blockCount; j++)
		{
			if (i & 1)
			{
				if (j & 1)
					images[i][j].set(brownImage);
				else
					images[i][j].set(whiteImage);
			}
			else
			{
				if (j & 1)
					images[i][j].set(whiteImage);
				else
					images[i][j].set(brownImage);
			}
		}
	}

	width -= trail;
	height -= trail;

	resize(width, height);

	return false;
}
