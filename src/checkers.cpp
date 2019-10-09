#include "checkers.h"

Checkers::Checkers()
{
	borderWidth = 2;
	width = 920;
	height = 920;
	blockWidth = width / 8;
	width = width + 2 * borderWidth;
	height = height + 2 * borderWidth;

	// headerBar.set_title("Checkers");
	// headerBar.set_has_subtitle(false);
	// headerBar.set_size_request(-1, 50);

	set_title("Checkers");
	set_default_size(width, height);
	set_border_width(borderWidth);
	override_background_color(Gdk::RGBA("black"), Gtk::STATE_FLAG_NORMAL);

	// set_titlebar(headerBar);

	add(scrolledWindow);

	// myButton.set_label("Click");
	// myButton.signal_clicked().connect(sigc::bind<Glib::ustring>(sigc::mem_fun(*this, &Checkers::onButtonClicked), "Button 1"));
	// myGrid.add(myButton);

	add_events(Gdk::STRUCTURE_MASK);
	signal_configure_event().connect(sigc::mem_fun(*this, &Checkers::onConfigureChanged), false);

	blockBrownOriginal = Gdk::Pixbuf::create_from_file("./resources/block_brown.jpg");
	blockWhiteOriginal = Gdk::Pixbuf::create_from_file("./resources/block_white.jpg");
	stoneBlackOriginal = Gdk::Pixbuf::create_from_file("./resources/stone_brown.jpg");
	stoneWhiteOriginal = Gdk::Pixbuf::create_from_file("./resources/stone_white.jpg");
	queenBlackOriginal = Gdk::Pixbuf::create_from_file("./resources/queen_brown.jpg");
	queenWhiteOriginal = Gdk::Pixbuf::create_from_file("./resources/queen_white.jpg");
	stoneBlackSelectOriginal = Gdk::Pixbuf::create_from_file("./resources/stone_brown_select.jpg");
	stoneWhiteSelectOriginal = Gdk::Pixbuf::create_from_file("./resources/stone_white_select.jpg");
	queenBlackSelectOriginal = Gdk::Pixbuf::create_from_file("./resources/queen_brown_select.jpg");
	queenWhiteSelectOriginal = Gdk::Pixbuf::create_from_file("./resources/queen_white_select.jpg");
	moveCircleOriginal = Gdk::Pixbuf::create_from_file("./resources/move.jpg");
	jumpCircleOriginal = Gdk::Pixbuf::create_from_file("./resources/jump.jpg");

	blockBrown = blockBrownOriginal;
	blockWhite = blockWhiteOriginal;
	stoneBlack = stoneBlackOriginal;
	stoneWhite = stoneWhiteOriginal;
	queenBlack = queenBlackOriginal;
	queenWhite = queenWhiteOriginal;
	stoneBlackSelect = stoneBlackSelectOriginal;
	stoneWhiteSelect = stoneWhiteSelectOriginal;
	queenBlackSelect = queenBlackSelectOriginal;
	queenWhiteSelect = queenWhiteSelectOriginal;
	moveCircle = moveCircleOriginal;
	jumpCircle = jumpCircleOriginal;

	for (int i = 0; i < blockCount; i++)
	{
		for (int j = 0; j < blockCount; j++)
		{
			string message = to_string(i) + to_string(j);

			if (i & 1)
			{
				if (j & 1)
					images[i][j].set(blockBrown);
				else
					images[i][j].set(blockWhite);
			}
			else
			{
				if (j & 1)
					images[i][j].set(blockWhite);
				else
					images[i][j].set(blockBrown);
			}

			// images[i][j].set_hexpand(true);
			// images[i][j].set_vexpand(true);
			// eventBoxes[i][j].set_halign(Gtk::ALIGN_FILL);
			// eventBoxes[i][j].set_valign(Gtk::ALIGN_FILL);
			eventBoxes[i][j].add(images[i][j]);
			eventBoxes[i][j].set_events(Gdk::BUTTON_PRESS_MASK);
			eventBoxes[i][j].signal_button_press_event().connect(sigc::bind<Glib::ustring>(sigc::mem_fun(*this, &Checkers::onEventboxButtonPress), message));

			myGrid.attach(eventBoxes[i][j], i, j, 1, 1);
		}
	}

	scrolledWindow.add(myGrid);

	show_all_children();
}

Checkers::~Checkers()
{
}

void Checkers::refreshBoard()
{
	for (int i = 0; i < blockCount; i++)
	{
		for (int j = 0; j < blockCount; j++)
		{
			if (i & 1)
			{
				if (j & 1)
					images[i][j].set(blockBrown);
				else
					images[i][j].set(blockWhite);
			}
			else
			{
				if (j & 1)
					images[i][j].set(blockWhite);
				else
					images[i][j].set(blockBrown);
			}
		}
	}
}

void Checkers::onButtonClicked(Glib::ustring data)
{
	std::cout << data << " was pressed" << std::endl;
}

bool Checkers::onEventboxButtonPress(GdkEventButton * /*button_event*/, Glib::ustring data)
{
	int x;
	int y;

	switch (data[0])
	{
		case '0':
			x = 0;
			break;
		case '1':
			x = 1;
			break;
		case '2':
			x = 2;
			break;
		case '3':
			x = 3;
			break;
		case '4':
			x = 4;
			break;
		case '5':
			x = 5;
			break;
		case '6':
			x = 6;
			break;
		default:
			x = 7;
			break;
	}

	switch (data[1])
	{
		case '0':
			y = 0;
			break;
		case '1':
			y = 1;
			break;
		case '2':
			y = 2;
			break;
		case '3':
			y = 3;
			break;
		case '4':
			y = 4;
			break;
		case '5':
			y = 5;
			break;
		case '6':
			y = 6;
			break;
		default:
			y = 7;
			break;
	}

	cout << x << ", " << y << endl;

	// hide();
	return true;
}

bool Checkers::onConfigureChanged(GdkEventConfigure * event)
{
	int newWidth = event->width;
	int newHeight = event->height;

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
	
	int chunk = (height - 2 * borderWidth) / 8;
	int trail = (height - 2 * borderWidth) % 8;

	blockBrown = blockBrownOriginal->scale_simple(chunk, chunk, Gdk::INTERP_BILINEAR);
	blockWhite = blockWhiteOriginal->scale_simple(chunk, chunk, Gdk::INTERP_BILINEAR);
	stoneBlack = stoneBlackOriginal->scale_simple(chunk, chunk, Gdk::INTERP_BILINEAR);
	stoneWhite = stoneWhiteOriginal->scale_simple(chunk, chunk, Gdk::INTERP_BILINEAR);
	queenBlack = queenBlackOriginal->scale_simple(chunk, chunk, Gdk::INTERP_BILINEAR);
	queenWhite = queenWhiteOriginal->scale_simple(chunk, chunk, Gdk::INTERP_BILINEAR);
	stoneBlackSelect = stoneBlackSelectOriginal->scale_simple(chunk, chunk, Gdk::INTERP_BILINEAR);
	stoneWhiteSelect = stoneWhiteSelectOriginal->scale_simple(chunk, chunk, Gdk::INTERP_BILINEAR);
	queenBlackSelect = queenBlackSelectOriginal->scale_simple(chunk, chunk, Gdk::INTERP_BILINEAR);
	queenWhiteSelect = queenWhiteSelectOriginal->scale_simple(chunk, chunk, Gdk::INTERP_BILINEAR);
	moveCircle = moveCircleOriginal->scale_simple(chunk, chunk, Gdk::INTERP_BILINEAR);
	jumpCircle = jumpCircleOriginal->scale_simple(chunk, chunk, Gdk::INTERP_BILINEAR);

	refreshBoard();

	width -= trail;
	height -= trail;

	resize(width, height);

	return false;
}
