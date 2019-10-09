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

	initArrays();

	for (int i = 0; i < blockCount; i++)
	{
		for (int j = 0; j < blockCount; j++)
		{
			string message = to_string(i) + to_string(j);

			refreshBlockImage(i, j);

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

void Checkers::initArrays()
{
	selected = false;                                   //  no cell is selected
    white_playing = true;                               //  first player is on turn at start of game
    only_jumping = 0;                                   //  insane to jump at the beginning of game
    turn_started = false;                               //  multiple jump is not possible at the beginning of game

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
            whites[i][j] = blacks[i][j] = queens[i][j] = false;     //  clear all description arrays
    }

    whites[0][1] = true;                                //  set default positions of all stones
    whites[0][3] = true;
    whites[0][5] = true;
    whites[0][7] = true;
    whites[1][0] = true;
    whites[1][2] = true;
    whites[1][4] = true;
    whites[1][6] = true;
    whites[2][1] = true;
    whites[2][3] = true;
    whites[2][5] = true;
    whites[2][7] = true;

    blacks[5][0] = true;
    blacks[5][2] = true;
    blacks[5][4] = true;
    blacks[5][6] = true;
    blacks[6][1] = true;
    blacks[6][3] = true;
    blacks[6][5] = true;
    blacks[6][7] = true;
    blacks[7][0] = true;
    blacks[7][2] = true;
    blacks[7][4] = true;
    blacks[7][6] = true;

    game_over = false;                                  //  game is not over by default
}

void Checkers::refreshBlockImage(int & x, int & y)
{
	if (whites[y][x])                                   //  white stone have to be on this cell
    {
        if (queens[y][x])                               //  queen
            images[x][y].set(queenWhite);       		//  load white queen image on this cell
        else                                            //  stone
            images[x][y].set(stoneWhite);       		//  load white stone image on this cell
    }
    else if (blacks[y][x])                              //  black stone have to be on this cell
    {
        if (queens[y][x])                               //  queen
            images[x][y].set(queenBlack);       		//  load black queen image onto this cell
        else                                            //  stone
            images[x][y].set(stoneBlack);       		//  load black stone image onto this cell
    }
    else                                                //  this cell have to be empty
    {
        if (x & 1)
		{
			if (y & 1)
				images[x][y].set(blockWhite);
			else
				images[x][y].set(blockBrown);
		}
		else
		{
			if (y & 1)
				images[x][y].set(blockBrown);
			else
				images[x][y].set(blockWhite);
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

	for (int i = 0; i < blockCount; i++)
	{
		for (int j = 0; j < blockCount; j++)
			refreshBlockImage(i, j);
	}

	width -= trail;
	height -= trail;

	resize(width, height);

	return false;
}
