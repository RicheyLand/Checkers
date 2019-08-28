#ifndef RICHERS_H
#define RICHERS_H

#include <QtWidgets>
#include <deque>
using namespace std;

enum
{
    game,                                               //  represents game tab index to tab widget
    menu,                                               //  represents menu tab index to tab widget
    about,                                              //  represents about tab index to tab widget
    help                                                //  represents help tab index to tab widget
};

namespace Ui
{
    class richers;
}

class ClickableLabel : public QLabel                    //  class which describes clickable label
{
    Q_OBJECT

public:
    explicit ClickableLabel(const QString & text = "", QWidget * parent = nullptr);
    int line;                                           //  line index
    int field;                                          //  field index
    ~ClickableLabel();

signals:
    void clicked(int, int);                             //  signal with line and field coordinates which is sent when label is clicked

protected:
    void mousePressEvent(QMouseEvent * event);          //  handle mouse press event
};

class coordinates                                       //  class which represents matrix coordinates
{
public:
    int x;                                              //  domain value
    int y;                                              //  relace value
};

class richers : public QWidget                          //  class which describes logic of richers game
{
    Q_OBJECT

public:
    explicit richers(QWidget * parent = nullptr);
    ~richers();

private:
    Ui::richers * ui;

    bool high_resolution;                               //  holds if application is running in the high resolution

    bool white_playing;                                 //  holds which player is on turn
    bool whites[8][8];                                  //  locations of white stones
    bool blacks[8][8];                                  //  locations of black stones
    bool queens[8][8];                                  //  locations of queens
    bool selected;                                      //  flag if red selection cell is active
    int only_jumping;                                   //  0 = move(any); 1 = stone jump; 2 = queen jump
    bool turn_started;                                  //  prevents selecting another jump stone when multiple jump has been started

    bool game_over;                                     //  holds if game is over
    int workspace;                                      //  holds actual tab widget index

    coordinates select;                                 //  red selection cell coordinates
    deque<coordinates> move_hint;                       //  list of green move hint cells coordinates
    deque<coordinates> stones_to_remove;                //  list of stones that are going to be removed after jump

    QPalette white_palette;                             //  white background palette
    QPalette black_palette;                             //  black background palette

    QPixmap * block_brown;                              //  image of empty brown block
    QPixmap * block_white;                              //  image of empty white block
    QPixmap * stone_black;                              //  image of black stone on brown block
    QPixmap * stone_white;                              //  image of white stone on brown block
    QPixmap * queen_black;                              //  image of black queen on brown block
    QPixmap * queen_white;                              //  image of white queen on brown block
    QPixmap * stone_black_select;                       //  image of selected black stone on brown block
    QPixmap * stone_white_select;                       //  image of selected white stone on brown block
    QPixmap * queen_black_select;                       //  image of selected black queen on brown block
    QPixmap * queen_white_select;                       //  image of selected white queen on brown block
    QPixmap * move_circle;                              //  image of blue move circle on brown block
    QPixmap * jump_circle;                              //  image of red jump circle on brown block

    QPixmap * new_game_image;                           //  new game button image
    QPixmap * help_image;                               //  help button image
    QPixmap * info_image;                               //  info button image
    QPixmap * exit_image;                               //  exit button image

    ClickableLabel block[8][8];                         //  array of clickable labels which represent gaming board with stones
    ClickableLabel menu_button[4];                      //  clickable labels represent menu buttons

    void keyPressEvent(QKeyEvent *);                    //	reaction to the keyboard press

    void create_colors();                               //  create all colors and palettes
    void init_table();                                  //  allocate memory for table widget items
    void create_icons();                                //  allocate memory for icons
    void init_arrays();                                 //  initialize stones locations
    void init_icons();                                  //  load icons on the desired table cells
    void init_menu();                                   //  initialize menu buttons
    void reset_board_color(int &, int &);               //  reset brown color on the desired cell

    void switch_to_game();                              //  move to game tab in the tab widget
    void switch_to_menu();                              //  move to menu tab in the tab widget

    void jump_predicate();                              //  recognize if is it possible to jump with some stones
    bool is_game_over();                                //  check if some player is winner
private slots:
    void click_reaction(int, int);                      //  reaction to the mouse click on desired cell
    void new_game_button_click();                       //  handle new game button press
    void help_button_click();                           //  handle help button press
    void about_button_click();                          //  handle about button press
    void exit_button_click();                           //  handle exit button press
};

#endif // RICHERS_H
