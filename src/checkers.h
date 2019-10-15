#ifndef CHECKERS_H
#define CHECKERS_H

#include <gtkmm.h>
#include <iostream>
using namespace std;

class Coordinates                                               //  class which represents matrix coordinates
{
public:
    int x;                                                      //  domain value
    int y;                                                      //  relace value
};

class Checkers : public Gtk::Window                             //  implements game window
{
public:
    Checkers();
    virtual ~Checkers();

private:
    bool whitePlaying;                                          //  holds which player is on turn
    bool whites[8][8];                                          //  locations of white stones
    bool blacks[8][8];                                          //  locations of black stones
    bool queens[8][8];                                          //  locations of queens
    bool selected;                                              //  flag if red selection cell is active
    int onlyJumping;                                            //  0 = move(any); 1 = stone jump; 2 = queen jump
    bool turnStarted;                                           //  prevents selecting another jump stone when multiple jump has been started
    bool gameOver;                                              //  game over flag

    Coordinates select;                                         //  red selection cell coordinates
    deque<Coordinates> moveHint;                                //  list of green move hint cells coordinates
    deque<Coordinates> stonesToRemove;                          //  list of stones that are going to be removed after jump

    static const int blockCount = 8;                            //  number of game board blocks in one column
    int borderWidth;                                            //  window border width value
    int blockWidth;                                             //  game board block width value
    int width;                                                  //  width value of the window
    int height;                                                 //  height value of the window

    Gtk::HeaderBar headerBar;
    
    Gtk::ScrolledWindow scrolledWindow;                         //  scrolled window wrapper
    Gtk::Grid myGrid;                                           //  grid which holds all the game board blocks

    Gtk::Image images[blockCount][blockCount];                  //  matrix of game board block images
    Gtk::EventBox eventBoxes[blockCount][blockCount];           //  bounding event box for all block image

    Glib::RefPtr<Gdk::Pixbuf> blockBrownOriginal;               //  pixel buffers of all required game images(original size)
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

    Glib::RefPtr<Gdk::Pixbuf> blockBrown;                       //  pixel buffers of all required game images(scaled size)
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

    void initArrays();                                          //  initialize game board data to the default state
    void resetBoardColor(int &, int &);                         //  update image of desired board block
    void jumpPredicate();                                       //  calculates possible range of stone jumping
    bool isGameOver();                                          //  calculates if the game is finally over
    void clickReaction(int, int);                               //  implements reaction to the game board click

    bool onEventboxButtonPress(GdkEventButton *, Glib::ustring);    //  event box click implementation
    bool onConfigureChanged(GdkEventConfigure *);               //  window resize is handled by this method
};

#endif
