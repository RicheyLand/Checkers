#ifndef CHECKERS_H
#define CHECKERS_H

#include <gtkmm.h>
#include <iostream>
using namespace std;

class Coordinates                                       //  class which represents matrix coordinates
{
public:
    int x;                                              //  domain value
    int y;                                              //  relace value
};

class Checkers : public Gtk::Window
{
public:
	Checkers();
	virtual ~Checkers();

private:
	bool white_playing;                                 //  holds which player is on turn
    bool whites[8][8];                                  //  locations of white stones
    bool blacks[8][8];                                  //  locations of black stones
    bool queens[8][8];                                  //  locations of queens
    bool selected;                                      //  flag if red selection cell is active
    int only_jumping;                                   //  0 = move(any); 1 = stone jump; 2 = queen jump
    bool turn_started;                                  //  prevents selecting another jump stone when multiple jump has been started
    bool game_over;                                     //  game over flag

    Coordinates select;                                 //  red selection cell coordinates
    deque<Coordinates> move_hint;                       //  list of green move hint cells coordinates
    deque<Coordinates> stones_to_remove;                //  list of stones that are going to be removed after jump


	static const int blockCount = 8;
	int borderWidth;
	int blockWidth;
	int width;
	int height;

	// Gtk::HeaderBar headerBar;
	
	Gtk::ScrolledWindow scrolledWindow;
	Gtk::Grid myGrid;
	Gtk::Button myButton;

	Gtk::Image images[blockCount][blockCount];
	Gtk::EventBox eventBoxes[blockCount][blockCount];

	Glib::RefPtr<Gdk::Pixbuf> blockBrownOriginal;
	Glib::RefPtr<Gdk::Pixbuf> blockWhiteOriginal;
	Glib::RefPtr<Gdk::Pixbuf> stoneBlackOriginal;
	Glib::RefPtr<Gdk::Pixbuf> stoneWhiteOriginal;
	Glib::RefPtr<Gdk::Pixbuf> queenBlackOriginal;
	Glib::RefPtr<Gdk::Pixbuf> queenWhiteOriginal;
	Glib::RefPtr<Gdk::Pixbuf> stoneBlackSelectOriginal;
	Glib::RefPtr<Gdk::Pixbuf> stoneWhiteSelectOriginal;
	Glib::RefPtr<Gdk::Pixbuf> queenBlackSelectOriginal;
	Glib::RefPtr<Gdk::Pixbuf> queenWhiteSelectOriginal;
	Glib::RefPtr<Gdk::Pixbuf> moveCircleOriginal;
	Glib::RefPtr<Gdk::Pixbuf> jumpCircleOriginal;

	Glib::RefPtr<Gdk::Pixbuf> blockBrown;
	Glib::RefPtr<Gdk::Pixbuf> blockWhite;
	Glib::RefPtr<Gdk::Pixbuf> stoneBlack;
	Glib::RefPtr<Gdk::Pixbuf> stoneWhite;
	Glib::RefPtr<Gdk::Pixbuf> queenBlack;
	Glib::RefPtr<Gdk::Pixbuf> queenWhite;
	Glib::RefPtr<Gdk::Pixbuf> stoneBlackSelect;
	Glib::RefPtr<Gdk::Pixbuf> stoneWhiteSelect;
	Glib::RefPtr<Gdk::Pixbuf> queenBlackSelect;
	Glib::RefPtr<Gdk::Pixbuf> queenWhiteSelect;
	Glib::RefPtr<Gdk::Pixbuf> moveCircle;
	Glib::RefPtr<Gdk::Pixbuf> jumpCircle;

	void refreshBoard();

	void onButtonClicked(Glib::ustring);
	bool onEventboxButtonPress(GdkEventButton *, Glib::ustring);
	bool onConfigureChanged(GdkEventConfigure *);
};

#endif
