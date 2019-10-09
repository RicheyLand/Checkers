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

            reset_board_color(i, j);

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

void Checkers::reset_board_color(int & x, int & y)
{
    if (whites[y][x])                                   //  white stone have to be on this cell
    {
        if (queens[y][x])                               //  queen
            images[x][y].set(queenWhite);               //  load white queen image on this cell
        else                                            //  stone
            images[x][y].set(stoneWhite);               //  load white stone image on this cell
    }
    else if (blacks[y][x])                              //  black stone have to be on this cell
    {
        if (queens[y][x])                               //  queen
            images[x][y].set(queenBlack);               //  load black queen image onto this cell
        else                                            //  stone
            images[x][y].set(stoneBlack);               //  load black stone image onto this cell
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

void Checkers::jump_predicate()
{
    only_jumping = 0;                                   //  set default jumping flag value

    if (white_playing)
    {
        for (int i = 0; i < 8; i++)                     //  jump check of white queens
        {
            for (int j = 0; j < 8; j++)
            {
                if (whites[i][j] && queens[i][j])
                {
                    int x = j, y = i;

                    while (x > 1 && y > 1)              //  check jumping left up
                    {
                        if (blacks[y - 1][x - 1] && blacks[y - 2][x - 2] == false &&
                            whites[y - 2][x - 2] == false)
                        {
                            only_jumping = 2;           //  set queen jumping flag
                            return;
                        }

                        if (blacks[y - 1][x - 1] == false && whites[y - 1][x - 1] == false)
                        {
                            x--;
                            y--;
                        }
                        else
                            break;
                    }

                    x = j;
                    y = i;

                    while (x < 6 && y > 1)              //  check jumping right up
                    {
                        if (blacks[y - 1][x + 1] && blacks[y - 2][x + 2] == false &&
                            whites[y - 2][x + 2] == false)
                        {
                            only_jumping = 2;           //  set queen jumping flag
                            return;
                        }

                        if (blacks[y - 1][x + 1] == false && whites[y - 1][x + 1] == false)
                        {
                            x++;
                            y--;
                        }
                        else
                            break;
                    }

                    x = j;
                    y = i;

                    while (x > 1 && y < 6)              //  check jumping left down
                    {
                        if (blacks[y + 1][x - 1] && blacks[y + 2][x - 2] == false &&
                            whites[y + 2][x - 2] == false)
                        {
                            only_jumping = 2;           //  set queen jumping flag
                            return;
                        }

                        if (blacks[y + 1][x - 1] == false && whites[y + 1][x - 1] == false)
                        {
                            x--;
                            y++;
                        }
                        else
                            break;
                    }

                    x = j;
                    y = i;

                    while (x < 6 && y < 6)              //  check jumping right down
                    {
                        if (blacks[y + 1][x + 1] && blacks[y + 2][x + 2] == false &&
                            whites[y + 2][x + 2] == false)
                        {
                            only_jumping = 2;           //  set queen jumping flag
                            return;
                        }

                        if (blacks[y + 1][x + 1] == false && whites[y + 1][x + 1] == false)
                        {
                            x++;
                            y++;
                        }
                        else
                            break;
                    }
                }
            }
        }

        for (int i = 0; i < 6; i++)                     //  jump check of white soldiers
        {
            for (int j = 0; j < 8; j++)
            {
                if (whites[i][j] && queens[i][j] == false)
                {
                    if (j > 1)                          //  check jumping left
                    {
                        if (blacks[i + 1][j - 1] == true && blacks[i + 2][j - 2] == false &&
                            whites[i + 2][j - 2] == false)
                        {
                            only_jumping = 1;           //  set soldier jumping flag
                            return;
                        }
                    }

                    if (j < 6)                          //  check jumping left
                    {
                        if (blacks[i + 1][j + 1] == true && blacks[i + 2][j + 2] == false &&
                            whites[i + 2][j + 2] == false)
                        {
                            only_jumping = 1;           //  set soldier jumping flag
                            return;
                        }
                    }
                }
            }
        }
    }
    else                                                //  jump check of black stones
    {
        for (int i = 0; i < 8; i++)                     //  jump check of black queens
        {
            for (int j = 0; j < 8; j++)
            {
                if (blacks[i][j] && queens[i][j])
                {
                    int x = j, y = i;

                    while (x > 1 && y > 1)              //  check jumping left up
                    {
                        if (whites[y - 1][x - 1] && whites[y - 2][x - 2] == false &&
                            blacks[y - 2][x - 2] == false)
                        {
                            only_jumping = 2;           //  set queen jumping flag
                            return;
                        }

                        if (whites[y - 1][x - 1] == false && blacks[y - 1][x - 1] == false)
                        {
                            x--;
                            y--;
                        }
                        else
                            break;
                    }

                    x = j;
                    y = i;

                    while (x < 6 && y > 1)              //  check jumping right up
                    {
                        if (whites[y - 1][x + 1] && whites[y - 2][x + 2] == false &&
                            blacks[y - 2][x + 2] == false)
                        {
                            only_jumping = 2;           //  set queen jumping flag
                            return;
                        }

                        if (whites[y - 1][x + 1] == false && blacks[y - 1][x + 1] == false)
                        {
                            x++;
                            y--;
                        }
                        else
                            break;
                    }

                    x = j;
                    y = i;

                    while (x > 1 && y < 6)              //  check jumping left down
                    {
                        if (whites[y + 1][x - 1] && whites[y + 2][x - 2] == false &&
                            blacks[y + 2][x - 2] == false)
                        {
                            only_jumping = 2;           //  set queen jumping flag
                            return;
                        }

                        if (whites[y + 1][x - 1] == false && blacks[y + 1][x - 1] == false)
                        {
                            x--;
                            y++;
                        }
                        else
                            break;
                    }

                    x = j;
                    y = i;

                    while (x < 6 && y < 6)              //  check jumping right down
                    {
                        if (whites[y + 1][x + 1] && whites[y + 2][x + 2] == false &&
                            blacks[y + 2][x + 2] == false)
                        {
                            only_jumping = 2;           //  set queen jumping flag
                            return;
                        }

                        if (whites[y + 1][x + 1] == false && blacks[y + 1][x + 1] == false)
                        {
                            x++;
                            y++;
                        }
                        else
                            break;
                    }
                }
            }
        }

        for (int i = 2; i < 8; i++)                     //  jump check of black soldiers
        {
            for (int j = 0; j < 8; j++)
            {
                if (blacks[i][j] && queens[i][j] == false)
                {
                    if (j > 1)                          //  check jumping left
                    {
                        if (whites[i - 1][j - 1] && whites[i - 2][j - 2] == false &&
                            blacks[i - 2][j - 2] == false)
                        {
                            only_jumping = 1;           //  set soldier jumping flag
                            return;
                        }
                    }

                    if (j < 6)                          //  check jumping right
                    {
                        if (whites[i - 1][j + 1] && whites[i - 2][j + 2] == false &&
                            blacks[i - 2][j + 2] == false)
                        {
                            only_jumping = 1;           //  set soldier jumping flag
                            return;
                        }
                    }
                }
            }
        }
    }
}

bool Checkers::is_game_over()
{
    if (only_jumping == 0)                              //  jump predicate is not active
    {
        if (white_playing)                              //  check game over of white player
        {
            for (int i = 0; i < 8; i++)
            {
                for (int j = 0; j < 8; j++)
                {
                    if (whites[i][j])
                    {
                        if (j && i < 7)                 //  check possible queen or soldier move left up
                        {
                            if (blacks[i + 1][j - 1] == false && whites[i + 1][j - 1] == false)
                                return false;
                        }

                        if (j < 7 && i < 7)             //  check possible queen or soldier move right up
                        {
                            if (blacks[i + 1][j + 1] == false && whites[i + 1][j + 1] == false)
                                return false;
                        }

                        if (queens[i][j])
                        {
                            if (j && i)                 //  check possible queen move left up
                            {
                                if (blacks[i - 1][j - 1] == false && whites[i - 1][j - 1] == false)
                                    return false;
                            }

                            if (j < 7 && i)             //  check possible queen move right up
                            {
                                if (blacks[i - 1][j + 1] == false && whites[i - 1][j + 1] == false)
                                    return false;
                            }
                        }
                    }
                }
            }
        }
        else                                            //  check game over of black player
        {
            for (int i = 0; i < 8; i++)
            {
                for (int j = 0; j < 8; j++)
                {
                    if (blacks[i][j])
                    {
                        if (j && i)                     //  check possible queen or soldier move left up
                        {
                            if (blacks[i - 1][j - 1] == false && whites[i - 1][j - 1] == false)
                                return false;
                        }

                        if (j < 7 && i)                 //  check possible queen or soldier move right up
                        {
                            if (blacks[i - 1][j + 1] == false && whites[i - 1][j + 1] == false)
                                return false;
                        }

                        if (queens[i][j])
                        {
                            if (j && i < 7)             //  check possible queen move left down
                            {
                                if (blacks[i + 1][j - 1] == false && whites[i + 1][j - 1] == false)
                                    return false;
                            }

                            if (j < 7 && i < 7)         //  check possible queen move right down
                            {
                                if (blacks[i + 1][j + 1] == false && whites[i + 1][j + 1] == false)
                                    return false;
                            }
                        }
                    }
                }
            }
        }

        game_over = true;
        return true;                                    //   end game state detected
    }

    return false;
}

void Checkers::click_reaction(int y, int x)
{
    if (white_playing)                                  //  white player is on turn
    {
        if (whites[y][x] && turn_started == false)      //  it has been clicked on the white stone and there is no running multiple jump
        {
            if (queens[y][x])                           //  there is white queen on the selected cell
            {
                bool flag = false;

                if (only_jumping == 2)                  //  jump with white queen is possible
                {
                    int old_x = x, old_y = y;

                    while (x > 1 && y > 1)              //  check jumping left up from actual cell
                    {
                        if (blacks[y - 1][x - 1] && blacks[y - 2][x - 2] == false &&
                            whites[y - 2][x - 2] == false)
                        {
                            flag = true;
                            break;
                        }

                        if (blacks[y - 1][x - 1] == false && whites[y - 1][x - 1] == false)
                        {
                            x--;
                            y--;
                        }
                        else
                            break;
                    }

                    x = old_x;
                    y = old_y;

                    if (flag == false)
                    {
                        while (x < 6 && y > 1)          //  check jumping right up from actual cell
                        {
                            if (blacks[y - 1][x + 1] && blacks[y - 2][x + 2] == false &&
                                whites[y - 2][x + 2] == false)
                            {
                                flag = true;
                                break;
                            }

                            if (blacks[y - 1][x + 1] == false && whites[y - 1][x + 1] == false)
                            {
                                x++;
                                y--;
                            }
                            else
                                break;
                        }

                        x = old_x;
                        y = old_y;

                        if (flag == false)
                        {
                            while (x > 1 && y < 6)      //  check jumping left down from actual cell
                            {
                                if (blacks[y + 1][x - 1] && blacks[y + 2][x - 2] == false &&
                                    whites[y + 2][x - 2] == false)
                                {
                                    flag = true;
                                    break;
                                }

                                if (blacks[y + 1][x - 1] == false && whites[y + 1][x - 1] == false)
                                {
                                    x--;
                                    y++;
                                }
                                else
                                    break;
                            }

                            x = old_x;
                            y = old_y;

                            if (flag == false)
                            {
                                while (x < 6 && y < 6)  //  check jumping right down from actual cell
                                {
                                    if (blacks[y + 1][x + 1] && blacks[y + 2][x + 2] == false &&
                                        whites[y + 2][x + 2] == false)
                                    {
                                        flag = true;
                                        break;
                                    }

                                    if (blacks[y + 1][x + 1] == false && whites[y + 1][x + 1] == false)
                                    {
                                        x++;
                                        y++;
                                    }
                                    else
                                        break;
                                }

                                x = old_x;              //  reset default x and y values
                                y = old_y;
                            }

                        }
                    }
                }

                if (only_jumping == 0)                  //  jumping with queen is not possible
                {
                    if (x && y)                         //  check moving left up with queen
                    {
                        if (blacks[y - 1][x - 1] == false && whites[y - 1][x - 1] == false)
                            flag = true;
                    }

                    if (flag == false && x < 7 && y)    //  check moving right up with queen
                    {
                        if (blacks[y - 1][x + 1] == false && whites[y - 1][x + 1] == false)
                            flag = true;
                    }

                    if (flag == false && x && y < 7)    //  check moving left down with queen
                    {
                        if (blacks[y + 1][x - 1] == false && whites[y + 1][x - 1] == false)
                            flag = true;
                    }

                    if (flag == false && x < 7 && y < 7)    //  check moving right down with queen
                    {
                        if (blacks[y + 1][x + 1] == false && whites[y + 1][x + 1] == false)
                            flag = true;
                    }
                }

                if (flag)
                {
                    if (selected)                       //  cancel actual green selected cell
                    {
                        reset_board_color(select.x, select.y);

                        do                              //  cancel actual move hint cells
                        {
                            reset_board_color(move_hint.front().x, move_hint.front().y);
                            move_hint.pop_front();
                        }while (move_hint.size());
                    }
                    else
                        selected = true;                //  set flag if white stone was not selected

                    select.x = x;                       //  save coordinates
                    select.y = y;

                    if (queens[y][x])
                        images[x][y].set(queenWhiteSelect);    //  set new cell with white stone as selected
                    else
                        images[x][y].set(stoneWhiteSelect);    //  set new cell with white queen as selected

                    if (only_jumping)                   //  jumping with queen is possible
                    {
                        int old_x = x, old_y = y;

                        while (x > 1 && y > 1)          //  prepare for jumping left up
                        {
                            if (blacks[y - 1][x - 1] && blacks[y - 2][x - 2] == false &&
                                whites[y - 2][x - 2] == false)
                            {
                                x -= 2;
                                y -= 2;

                                coordinates temp;       //  draw possible movement cell to red
                                temp.x = x;
                                temp.y = y;
                                move_hint.push_back(temp);
                                images[x][y].set(jumpCircle);

                                while (x && y)          //  draw all possible jump cells to red
                                {
                                    if (blacks[y - 1][x - 1] || whites[y - 1][x - 1])
                                        break;

                                    temp.x = --x;       //  draw possible movement cell to red
                                    temp.y = --y;
                                    move_hint.push_back(temp);
                                    images[x][y].set(jumpCircle);
                                }

                                break;
                            }

                            if (blacks[y - 1][x - 1] == false && whites[y - 1][x - 1] == false)
                            {
                                x--;
                                y--;
                            }
                            else
                                break;
                        }

                        x = old_x;
                        y = old_y;

                        while (x < 6 && y > 1)          //  prepare for jumping right up
                        {
                            if (blacks[y - 1][x + 1] && blacks[y - 2][x + 2] == false &&
                                whites[y - 2][x + 2] == false)
                            {
                                x += 2;
                                y -= 2;

                                coordinates temp;       //  draw possible movement cell to red
                                temp.x = x;
                                temp.y = y;
                                move_hint.push_back(temp);
                                images[x][y].set(jumpCircle);

                                while (x < 7 && y)      //  draw all possible jump cells to red
                                {
                                    if (blacks[y - 1][x + 1] || whites[y - 1][x + 1])
                                        break;

                                    temp.x = ++x;       //  draw possible movement cell to red
                                    temp.y = --y;
                                    move_hint.push_back(temp);
                                    images[x][y].set(jumpCircle);
                                }

                                break;
                            }

                            if (blacks[y - 1][x + 1] == false && whites[y - 1][x + 1] == false)
                            {
                                x++;
                                y--;
                            }
                            else
                                break;
                        }

                        x = old_x;
                        y = old_y;

                        while (x > 1 && y < 6)          //  prepare for jumping left down
                        {
                            if (blacks[y + 1][x - 1] && blacks[y + 2][x - 2] == false &&
                                whites[y + 2][x - 2] == false)
                            {
                                x -= 2;
                                y += 2;

                                coordinates temp;       //  draw possible movement cell to red
                                temp.x = x;
                                temp.y = y;
                                move_hint.push_back(temp);
                                images[x][y].set(jumpCircle);

                                while (x && y < 7)      //  draw all possible jump cells to red
                                {
                                    if (blacks[y + 1][x - 1] || whites[y + 1][x - 1])
                                        break;

                                    temp.x = --x;       //  draw possible movement cell to red
                                    temp.y = ++y;
                                    move_hint.push_back(temp);
                                    images[x][y].set(jumpCircle);
                                }

                                break;
                            }

                            if (blacks[y + 1][x - 1] == false && whites[y + 1][x - 1] == false)
                            {
                                x--;
                                y++;
                            }
                            else
                                break;
                        }

                        x = old_x;
                        y = old_y;

                        while (x < 6 && y < 6)          //  prepare for jumping right down
                        {
                            if (blacks[y + 1][x + 1] && blacks[y + 2][x + 2] == false &&
                                whites[y + 2][x + 2] == false)
                            {
                                x += 2;
                                y += 2;

                                coordinates temp;       //  draw possible movement cell to red
                                temp.x = x;
                                temp.y = y;
                                move_hint.push_back(temp);
                                images[x][y].set(jumpCircle);

                                while (x < 7 && y < 7)  //  draw all possible jump cells to red
                                {
                                    if (blacks[y + 1][x + 1] || whites[y + 1][x + 1])
                                        break;

                                    temp.x = ++x;       //  draw possible movement cell to red
                                    temp.y = ++y;
                                    move_hint.push_back(temp);
                                    images[x][y].set(jumpCircle);
                                }

                                break;
                            }

                            if (blacks[y + 1][x + 1] == false && whites[y + 1][x + 1] == false)
                            {
                                x++;
                                y++;
                            }
                            else
                                break;
                        }
                    }
                    else                                //  only moving with queen is possible
                    {
                        int old_x = x, old_y = y;

                        while (x && y)                  //  prepare for move left up
                        {
                            if (blacks[y - 1][x - 1] || whites[y - 1][x - 1])
                                break;

                            coordinates temp;           //  draw possible movement cell to blue
                            temp.x = --x;
                            temp.y = --y;
                            move_hint.push_back(temp);
                            images[x][y].set(moveCircle);
                        }

                        x = old_x;
                        y = old_y;

                        while (x < 7 && y)              //  prepare for move right up
                        {
                            if (blacks[y - 1][x + 1] || whites[y - 1][x + 1])
                                break;

                            coordinates temp;           //  draw possible movement cell to blue
                            temp.x = ++x;
                            temp.y = --y;
                            move_hint.push_back(temp);
                            images[x][y].set(moveCircle);
                        }

                        x = old_x;
                        y = old_y;

                        while (x && y < 7)              //  prepare for move left down
                        {
                            if (blacks[y + 1][x - 1] || whites[y + 1][x - 1])
                                break;

                            coordinates temp;           //  draw possible movement cell to blue
                            temp.x = --x;
                            temp.y = ++y;
                            move_hint.push_back(temp);
                            images[x][y].set(moveCircle);
                        }

                        x = old_x;
                        y = old_y;

                        while (x < 7 && y < 7)          //  prepare for move right down
                        {
                            if (blacks[y + 1][x + 1] || whites[y + 1][x + 1])
                                break;

                            coordinates temp;           //  draw possible movement cell to blue
                            temp.x = ++x;
                            temp.y = ++y;
                            move_hint.push_back(temp);
                            images[x][y].set(moveCircle);
                        }
                    }
                }
            }
            else                                        //  there is white soldier on selected cell
            {
                bool flag = false;                      //  tell if white stone can move from selected cell

                if (only_jumping == 1)
                {
                    if (x > 1 && y < 6)                 //  jumping left check
                    {
                        if (blacks[y + 1][x - 1] && blacks[y + 2][x - 2] == false && whites[y + 2][x - 2] == false)
                            flag = true;
                    }

                    if (flag == false && x < 6 && y < 6)    //  jumping right check
                    {
                        if (blacks[y + 1][x + 1] && blacks[y + 2][x + 2] == false && whites[y + 2][x + 2] == false)
                            flag = true;
                    }
                }

                if (only_jumping == 0)                  //  stone cannot move when jumping is possible
                {
                    if (x && y < 7)                     //  moving left check
                    {
                        if (blacks[y + 1][x - 1] == false && whites[y + 1][x - 1] == false)
                            flag = true;
                    }

                    if (flag == false && x < 7 && y < 7)    //  moving right check
                    {
                        if (blacks[y + 1][x + 1] == false && whites[y + 1][x + 1] == false)
                            flag = true;
                    }
                }

                if (flag)                               //  move or jump is possible from selected cell
                {
                    if (selected)                       //  cancel actual green selected cell
                    {
                        reset_board_color(select.x, select.y);

                        do                              //  cancel actual move hint cells
                        {
                            reset_board_color(move_hint.front().x, move_hint.front().y);
                            move_hint.pop_front();
                        }while (move_hint.size());
                    }
                    else
                        selected = true;                //  set flag if white stone was not selected

                    select.x = x;                       //  save coordinates
                    select.y = y;

                    if (queens[y][x])
                        images[x][y].set(queenWhiteSelect);    //  set new cell with white stone as selected
                    else
                        images[x][y].set(stoneWhiteSelect);    //  set new cell with white queen as selected

                    if (only_jumping)                   //  jumping is required when it is possible to jump
                    {
                        if (x > 1 && y < 6)             //  jumping left
                        {
                            if (blacks[y + 1][x - 1] && blacks[y + 2][x - 2] == false && whites[y + 2][x - 2] == false)
                            {
                                coordinates temp;       //  draw left move hint cell to red
                                temp.x = x - 2;
                                temp.y = y + 2;
                                move_hint.push_back(temp);

                                images[temp.x][temp.y].set(jumpCircle);
                            }
                        }

                        if (x < 6 && y < 6)             //  jumping right
                        {
                            if (blacks[y + 1][x + 1] && blacks[y + 2][x + 2] == false && whites[y + 2][x + 2] == false)
                            {
                                coordinates temp;       //  draw right move hint cell to red
                                temp.x = x + 2;
                                temp.y = y + 2;
                                move_hint.push_back(temp);

                                images[temp.x][temp.y].set(jumpCircle);
                            }
                        }
                    }
                    else                                //  stone cannot move when jumping is possible
                    {
                        if (x && y < 7)                 //  moving left
                        {
                            if (blacks[y + 1][x - 1] == false && whites[y + 1][x - 1] == false)
                            {
                                coordinates temp;       //  draw left move hint cell to blue
                                temp.x = x - 1;
                                temp.y = y + 1;
                                move_hint.push_back(temp);
                                images[temp.x][temp.y].set(moveCircle);
                            }
                        }

                        if (x < 7 && y < 7)             //  moving right
                        {
                            if (blacks[y + 1][x + 1] == false && whites[y + 1][x + 1] == false)
                            {
                                coordinates temp;       //  draw right move hint cell to blue
                                temp.x = x + 1;
                                temp.y = y + 1;
                                move_hint.push_back(temp);
                                images[temp.x][temp.y].set(moveCircle);
                            }
                        }
                    }
                }
            }
        }
        else if (selected)                              //  click on non-white stone cell when selected
        {
            unsigned long len = move_hint.size();

            for (unsigned long i = 0; i < len; i++)
            {                                           //  cell is in movement hints
                if (move_hint[i].x == x && move_hint[i].y == y)
                {
                    turn_started = true;                //  selecting another stone is disabled until actual turn will not end

                    images[select.x][select.y].set(blockBrown);

                    if (queens[select.y][select.x])
                    {
                        images[x][y].set(queenWhite);   //  refresh stones positions
                        queens[select.y][select.x] = false;
                        queens[y][x] = true;
                    }
                    else
                    {
                        if (y == 7)                     //  transform soldier to queen
                        {
                            queens[y][x] = true;
                            images[x][y].set(queenWhite);   //  set queen icon
                        }
                        else                            //  still soldier
                            images[x][y].set(stoneWhite);   //  refresh stones positions
                    }

                    whites[select.y][select.x] = false;
                    whites[y][x] = true;                //  refresh stones positions

                    reset_board_color(select.x, select.y);  //  clear green selection cell

                    do                                  //  cancel actual move hint cells
                    {
                        reset_board_color(move_hint.front().x, move_hint.front().y);
                        move_hint.pop_front();
                    }while (move_hint.size());

                    if (only_jumping)                   //  remove black stone if jumping
                    {
                        int diff_x = x - select.x;      //  length between cells
                        int diff_y = y - select.y;

                        if (diff_x > 0)                 //  count cells domain differention
                            diff_x = 1;
                        else
                            diff_x = -1;

                        if (diff_y > 0)                 //  count cells relace differention
                            diff_y = 1;
                        else
                            diff_y = -1;

                        while (true)                    //  loop will be stopped manually
                        {
                            select.x += diff_x;         //  iteratively find white stone to remove
                            select.y += diff_y;

                            if (blacks[select.y][select.x])
                            {
                                coordinates temp;       //  delete stone after complete jump
                                temp.x = select.x;      //  jump can be multiple
                                temp.y = select.y;
                                stones_to_remove.push_back(temp);   //  push to list

                                blacks[select.y][select.x] = false;     //  refresh blacks array
                                whites[select.y][select.x] = true;  //  disable overjumping of stone by this

                                break;                  //  if stone is queen then it will be removed after
                            }
                        }
                    }

                    if (only_jumping == 1)              //  soldier multiple jump checking
                    {
                        if (x > 1 && y < 6)
                        {
                            if (blacks[y + 1][x - 1] && blacks[y + 2][x - 2] == false && whites[y + 2][x - 2] == false)
                            {
                                coordinates temp;       //  draw left move hint cell to red
                                temp.x = x - 2;
                                temp.y = y + 2;
                                move_hint.push_back(temp);
                            }
                        }

                        if (x < 6 && y < 6)
                        {
                            if (blacks[y + 1][x + 1] && blacks[y + 2][x + 2] == false && whites[y + 2][x + 2] == false)
                            {
                                coordinates temp;       //  draw right move hint cell to red
                                temp.x = x + 2;
                                temp.y = y + 2;
                                move_hint.push_back(temp);
                            }
                        }

                        unsigned long len = move_hint.size();

                        if (len)                        //  multiple jump is possible
                        {
                            select.x = x;               //  save coordinates
                            select.y = y;

                            if (queens[y][x])
                                images[x][y].set(queenWhiteSelect);    //  set new cell with white stone as selected
                            else
                                images[x][y].set(stoneWhiteSelect);    //  set new cell with white queen as selected

                            for (unsigned long i = 0; i < len; i++)   //  draw all red move hint cells
                                images[move_hint[i].x][move_hint[i].y].set(jumpCircle);

                            return;                     //  white player is still playing
                        }
                    }
                    else if (only_jumping == 2)         //  queen multiple jump checking
                    {
                        int old_x = x, old_y = y;

                        while (x > 1 && y > 1)          //  prepare for jumping left up
                        {
                            if (blacks[y - 1][x - 1] && blacks[y - 2][x - 2] == false &&
                                whites[y - 2][x - 2] == false)
                            {
                                x -= 2;
                                y -= 2;

                                coordinates temp;       //  draw possible movement cell to red
                                temp.x = x;
                                temp.y = y;
                                move_hint.push_back(temp);

                                while (x && y)          //  draw all possible jump cells to red
                                {
                                    if (blacks[y - 1][x - 1] || whites[y - 1][x - 1])
                                        break;

                                    temp.x = --x;       //  draw possible movement cell to red
                                    temp.y = --y;
                                    move_hint.push_back(temp);
                                }

                                break;
                            }

                            if (blacks[y - 1][x - 1] == false && whites[y - 1][x - 1] == false)
                            {
                                x--;
                                y--;
                            }
                            else
                                break;
                        }

                        x = old_x;
                        y = old_y;

                        while (x < 6 && y > 1)          //  prepare for jumping right up
                        {
                            if (blacks[y - 1][x + 1] && blacks[y - 2][x + 2] == false &&
                                whites[y - 2][x + 2] == false)
                            {
                                x += 2;
                                y -= 2;

                                coordinates temp;       //  draw possible movement cell to red
                                temp.x = x;
                                temp.y = y;
                                move_hint.push_back(temp);

                                while (x < 7 && y)      //  draw all possible jump cells to red
                                {
                                    if (blacks[y - 1][x + 1] || whites[y - 1][x + 1])
                                        break;

                                    temp.x = ++x;       //  draw possible movement cell to red
                                    temp.y = --y;
                                    move_hint.push_back(temp);
                                }

                                break;
                            }

                            if (blacks[y - 1][x + 1] == false && whites[y - 1][x + 1] == false)
                            {
                                x++;
                                y--;
                            }
                            else
                                break;
                        }

                        x = old_x;
                        y = old_y;

                        while (x > 1 && y < 6)          //  prepare for jumping left down
                        {
                            if (blacks[y + 1][x - 1] && blacks[y + 2][x - 2] == false &&
                                whites[y + 2][x - 2] == false)
                            {
                                x -= 2;
                                y += 2;

                                coordinates temp;       //  draw possible movement cell to red
                                temp.x = x;
                                temp.y = y;
                                move_hint.push_back(temp);

                                while (x && y < 7)      //  draw all possible jump cells to red
                                {
                                    if (blacks[y + 1][x - 1] || whites[y + 1][x - 1])
                                        break;

                                    temp.x = --x;       //  draw possible movement cell to red
                                    temp.y = ++y;
                                    move_hint.push_back(temp);
                                }

                                break;
                            }

                            if (blacks[y + 1][x - 1] == false && whites[y + 1][x - 1] == false)
                            {
                                x--;
                                y++;
                            }
                            else
                                break;
                        }

                        x = old_x;
                        y = old_y;

                        while (x < 6 && y < 6)          //  prepare for jumping right down
                        {
                            if (blacks[y + 1][x + 1] && blacks[y + 2][x + 2] == false &&
                                whites[y + 2][x + 2] == false)
                            {
                                x += 2;
                                y += 2;

                                coordinates temp;       //  draw possible movement cell to red
                                temp.x = x;
                                temp.y = y;
                                move_hint.push_back(temp);

                                while (x < 7 && y < 7)  //  draw all possible jump cells to red
                                {
                                    if (blacks[y + 1][x + 1] || whites[y + 1][x + 1])
                                        break;

                                    temp.x = ++x;       //  draw possible movement cell to red
                                    temp.y = ++y;
                                    move_hint.push_back(temp);
                                }

                                break;
                            }

                            if (blacks[y + 1][x + 1] == false && whites[y + 1][x + 1] == false)
                            {
                                x++;
                                y++;
                            }
                            else
                                break;
                        }

                        x = old_x;
                        y = old_y;

                        unsigned long len = move_hint.size();

                        if (len)                        //  multiple jump is possible
                        {
                            select.x = x;               //  save coordinates
                            select.y = y;

                            if (queens[y][x])
                                images[x][y].set(queenWhiteSelect);    //  set new cell with white stone as selected
                            else
                                images[x][y].set(stoneWhiteSelect);    //  set new cell with white queen as selected

                            for (unsigned long i = 0; i < len; i++)   //  draw all red move hint cells
                                images[move_hint[i].x][move_hint[i].y].set(jumpCircle);

                            return;                     //  white player is still playing
                        }
                    }

                    while (stones_to_remove.size())     //  remove all overjumped opponents stones
                    {
                        int temp_x = stones_to_remove.front().x;    //  save temporary coordinates
                        int temp_y = stones_to_remove.front().y;

                        images[temp_x][temp_y].set(blockBrown);
                        whites[temp_y][temp_x] = false;

                        if (queens[temp_y][temp_x])
                            queens[temp_y][temp_x] = false;     //  physically remove queen

                        stones_to_remove.pop_front();   //  now remove all overjumped stones physically
                    }

                    selected = false;                   //  remove selection flag
                    white_playing = false;              //  black player will be on turn
                    turn_started = false;               //  selecting another stone is enabled from this moment

                    jump_predicate();                   //  predicate jump situations

                    return;
                }
            }
        }
    }
    else                                                //  black player is on turn
    {
        if (blacks[y][x] && turn_started == false)      //  it was clicked on black stone and there is no running multiple jump
        {
            if (queens[y][x])                           //  there is black queen on selected cell
            {
                bool flag = false;

                if (only_jumping == 2)                  //  jump with black queen is possible
                {
                    int old_x = x, old_y = y;

                    while (x > 1 && y > 1)              //  check jumping left up from actual cell
                    {
                        if (whites[y - 1][x - 1] && whites[y - 2][x - 2] == false &&
                            blacks[y - 2][x - 2] == false)
                        {
                            flag = true;
                            break;
                        }

                        if (whites[y - 1][x - 1] == false && blacks[y - 1][x - 1] == false)
                        {
                            x--;
                            y--;
                        }
                        else
                            break;
                    }

                    x = old_x;
                    y = old_y;

                    if (flag == false)
                    {
                        while (x < 6 && y > 1)          //  check jumping right up from actual cell
                        {
                            if (whites[y - 1][x + 1] && whites[y - 2][x + 2] == false &&
                                blacks[y - 2][x + 2] == false)
                            {
                                flag = true;
                                break;
                            }

                            if (whites[y - 1][x + 1] == false && blacks[y - 1][x + 1] == false)
                            {
                                x++;
                                y--;
                            }
                            else
                                break;
                        }

                        x = old_x;
                        y = old_y;

                        if (flag == false)
                        {
                            while (x > 1 && y < 6)      //  check jumping left down from actual cell
                            {
                                if (whites[y + 1][x - 1] && whites[y + 2][x - 2] == false &&
                                    blacks[y + 2][x - 2] == false)
                                {
                                    flag = true;
                                    break;
                                }

                                if (whites[y + 1][x - 1] == false && blacks[y + 1][x - 1] == false)
                                {
                                    x--;
                                    y++;
                                }
                                else
                                    break;
                            }

                            x = old_x;
                            y = old_y;

                            if (flag == false)
                            {
                                while (x < 6 && y < 6)  //  check jumping right down from actual cell
                                {
                                    if (whites[y + 1][x + 1] && whites[y + 2][x + 2] == false &&
                                        blacks[y + 2][x + 2] == false)
                                    {
                                        flag = true;
                                        break;
                                    }

                                    if (whites[y + 1][x + 1] == false && blacks[y + 1][x + 1] == false)
                                    {
                                        x++;
                                        y++;
                                    }
                                    else
                                        break;
                                }

                                x = old_x;              //  reset default x and y values
                                y = old_y;
                            }
                        }
                    }
                }

                if (only_jumping == 0)                  //  jumping with queen is not possible
                {
                    if (x && y)                         //  check moving left up with queen
                    {
                        if (whites[y - 1][x - 1] == false && blacks[y - 1][x - 1] == false)
                            flag = true;
                    }

                    if (flag == false && x < 7 && y)    //  check moving right up with queen
                    {
                        if (whites[y - 1][x + 1] == false && blacks[y - 1][x + 1] == false)
                            flag = true;
                    }

                    if (flag == false && x && y < 7)    //  check moving left down with queen
                    {
                        if (whites[y + 1][x - 1] == false && blacks[y + 1][x - 1] == false)
                            flag = true;
                    }

                    if (flag == false && x < 7 && y < 7)    //  check moving right down with queen
                    {
                        if (whites[y + 1][x + 1] == false && blacks[y + 1][x + 1] == false)
                            flag = true;
                    }
                }

                if (flag)
                {
                    if (selected)                       //  cancel actual green selected cell
                    {
                        reset_board_color(select.x, select.y);

                        do                              //  cancel actual move hint cells
                        {
                            reset_board_color(move_hint.front().x, move_hint.front().y);
                            move_hint.pop_front();
                        }while (move_hint.size());
                    }
                    else
                        selected = true;                //  set flag if black stone was not selected

                    select.x = x;                       //  save coordinates
                    select.y = y;

                    if (queens[y][x])
                        images[x][y].set(queenBlackSelect);    //  set new cell with black stone as selected
                    else
                        images[x][y].set(stoneBlackSelect);    //  set new cell with black queen as selected

                    if (only_jumping)                   //  jumping with queen is possible
                    {
                        int old_x = x, old_y = y;

                        while (x > 1 && y > 1)          //  prepare for jumping left up
                        {
                            if (whites[y - 1][x - 1] && whites[y - 2][x - 2] == false &&
                                blacks[y - 2][x - 2] == false)
                            {
                                x -= 2;
                                y -= 2;

                                coordinates temp;       //  draw possible movement cell to red
                                temp.x = x;
                                temp.y = y;
                                move_hint.push_back(temp);
                                images[x][y].set(jumpCircle);

                                while (x && y)          //  draw all possible jump cells to red
                                {
                                    if (whites[y - 1][x - 1] || blacks[y - 1][x - 1])
                                        break;

                                    temp.x = --x;       //  draw possible movement cell to red
                                    temp.y = --y;
                                    move_hint.push_back(temp);
                                    images[x][y].set(jumpCircle);
                                }

                                break;
                            }

                            if (whites[y - 1][x - 1] == false && blacks[y - 1][x - 1] == false)
                            {
                                x--;
                                y--;
                            }
                            else
                                break;
                        }

                        x = old_x;
                        y = old_y;

                        while (x < 6 && y > 1)          //  prepare for jumping right up
                        {
                            if (whites[y - 1][x + 1] && whites[y - 2][x + 2] == false &&
                                blacks[y - 2][x + 2] == false)
                            {
                                x += 2;
                                y -= 2;

                                coordinates temp;       //  draw possible movement cell to red
                                temp.x = x;
                                temp.y = y;
                                move_hint.push_back(temp);
                                images[x][y].set(jumpCircle);

                                while (x < 7 && y)      //  draw all possible jump cells to red
                                {
                                    if (whites[y - 1][x + 1] || blacks[y - 1][x + 1])
                                        break;

                                    temp.x = ++x;       //  draw possible movement cell to red
                                    temp.y = --y;
                                    move_hint.push_back(temp);
                                    images[x][y].set(jumpCircle);
                                }

                                break;
                            }

                            if (whites[y - 1][x + 1] == false && blacks[y - 1][x + 1] == false)
                            {
                                x++;
                                y--;
                            }
                            else
                                break;
                        }

                        x = old_x;
                        y = old_y;

                        while (x > 1 && y < 6)          //  prepare for jumping left down
                        {
                            if (whites[y + 1][x - 1] && whites[y + 2][x - 2] == false &&
                                blacks[y + 2][x - 2] == false)
                            {
                                x -= 2;
                                y += 2;

                                coordinates temp;       //  draw possible movement cell to red
                                temp.x = x;
                                temp.y = y;
                                move_hint.push_back(temp);
                                images[x][y].set(jumpCircle);

                                while (x && y < 7)      //  draw all possible jump cells to red
                                {
                                    if (whites[y + 1][x - 1] || blacks[y + 1][x - 1])
                                        break;

                                    temp.x = --x;       //  draw possible movement cell to red
                                    temp.y = ++y;
                                    move_hint.push_back(temp);
                                    images[x][y].set(jumpCircle);
                                }

                                break;
                            }

                            if (whites[y + 1][x - 1] == false && blacks[y + 1][x - 1] == false)
                            {
                                x--;
                                y++;
                            }
                            else
                                break;
                        }

                        x = old_x;
                        y = old_y;

                        while (x < 6 && y < 6)          //  prepare for jumping right down
                        {
                            if (whites[y + 1][x + 1] && whites[y + 2][x + 2] == false &&
                                blacks[y + 2][x + 2] == false)
                            {
                                x += 2;
                                y += 2;

                                coordinates temp;       //  draw possible movement cell to red
                                temp.x = x;
                                temp.y = y;
                                move_hint.push_back(temp);
                                images[x][y].set(jumpCircle);

                                while (x < 7 && y < 7)  //  draw all possible jump cells to red
                                {
                                    if (whites[y + 1][x + 1] || blacks[y + 1][x + 1])
                                        break;

                                    temp.x = ++x;       //  draw possible movement cell to red
                                    temp.y = ++y;
                                    move_hint.push_back(temp);
                                    images[x][y].set(jumpCircle);
                                }

                                break;
                            }

                            if (whites[y + 1][x + 1] == false && blacks[y + 1][x + 1] == false)
                            {
                                x++;
                                y++;
                            }
                            else
                                break;
                        }
                    }
                    else                                //  only moving with queen is possible
                    {
                        int old_x = x, old_y = y;

                        while (x && y)                  //  prepare for move left up
                        {
                            if (whites[y - 1][x - 1] || blacks[y - 1][x - 1])
                                break;

                            coordinates temp;           //  draw possible movement cell to blue
                            temp.x = --x;
                            temp.y = --y;
                            move_hint.push_back(temp);
                            images[x][y].set(moveCircle);
                        }

                        x = old_x;
                        y = old_y;

                        while (x < 7 && y)              //  prepare for move right up
                        {
                            if (whites[y - 1][x + 1] || blacks[y - 1][x + 1])
                                break;

                            coordinates temp;           //  draw possible movement cell to blue
                            temp.x = ++x;
                            temp.y = --y;
                            move_hint.push_back(temp);
                            images[x][y].set(moveCircle);
                        }

                        x = old_x;
                        y = old_y;

                        while (x && y < 7)              //  prepare for move left down
                        {
                            if (whites[y + 1][x - 1] || blacks[y + 1][x - 1])
                                break;

                            coordinates temp;           //  draw possible movement cell to blue
                            temp.x = --x;
                            temp.y = ++y;
                            move_hint.push_back(temp);
                            images[x][y].set(moveCircle);
                        }

                        x = old_x;
                        y = old_y;

                        while (x < 7 && y < 7)          //  prepare for move right down
                        {
                            if (whites[y + 1][x + 1] || blacks[y + 1][x + 1])
                                break;

                            coordinates temp;           //  draw possible movement cell to blue
                            temp.x = ++x;
                            temp.y = ++y;
                            move_hint.push_back(temp);
                            images[x][y].set(moveCircle);
                        }
                    }
                }
            }
            else                                        //  there is black soldier on selected cell
            {
                bool flag = false;                      //  tell if black stone can move from selected cell

                if (only_jumping == 1)
                {
                    if (x > 1 && y > 1)                 //  jumping left check
                    {
                        if (whites[y - 1][x - 1] && whites[y - 2][x - 2] == false && blacks[y - 2][x - 2] == false)
                            flag = true;
                    }

                    if (flag == false && x < 6 && y > 1)    //  jumping right check
                    {
                        if (whites[y - 1][x + 1] && whites[y - 2][x + 2] == false && blacks[y - 2][x + 2] == false)
                            flag = true;
                    }
                }

                if (only_jumping == 0)                  //  stone cannot move when jumping is possible
                {
                    if (x && y)                         //  moving left check
                    {
                        if (whites[y - 1][x - 1] == false && blacks[y - 1][x - 1] == false)
                            flag = true;
                    }

                    if (flag == false && x < 7 && y)    //  moving right check
                    {
                        if (whites[y - 1][x + 1] == false && blacks[y - 1][x + 1] == false)
                            flag = true;
                    }
                }

                if (flag)                               //  move or jump is possible from selected cell
                {
                    if (selected)                       //  cancel actual green selected cell
                    {
                        reset_board_color(select.x, select.y);

                        do                              //  cancel actual move hint cells
                        {
                            reset_board_color(move_hint.front().x, move_hint.front().y);
                            move_hint.pop_front();
                        }while (move_hint.size());
                    }
                    else
                        selected = true;                //  set flag if black stone was not selected

                    select.x = x;                       //  save coordinates
                    select.y = y;

                    if (queens[y][x])
                        images[x][y].set(queenBlackSelect);    //  set new cell with black stone as selected
                    else
                        images[x][y].set(stoneBlackSelect);    //  set new cell with black queen as selected

                    if (only_jumping)                   //  jumping is required when it is possible to jump
                    {
                        if (x > 1 && y > 1)             //  jumping left
                        {
                            if (whites[y - 1][x - 1] && whites[y - 2][x - 2] == false && blacks[y - 2][x - 2] == false)
                            {
                                coordinates temp;       //  draw left move hint cell to red
                                temp.x = x - 2;
                                temp.y = y - 2;
                                move_hint.push_back(temp);

                                images[temp.x][temp.y].set(jumpCircle);
                            }
                        }

                        if (x < 6 && y > 1)             //  jumping right
                        {
                            if (whites[y - 1][x + 1] && whites[y - 2][x + 2] == false && blacks[y - 2][x + 2] == false)
                            {
                                coordinates temp;       //  draw right move hint cell to red
                                temp.x = x + 2;
                                temp.y = y - 2;
                                move_hint.push_back(temp);

                                images[temp.x][temp.y].set(jumpCircle);
                            }
                        }
                    }
                    else                                //  stone cannot move when jumping is possible
                    {
                        if (x && y)                     //  moving left
                        {
                            if (whites[y - 1][x - 1] == false && blacks[y - 1][x - 1] == false)
                            {
                                coordinates temp;       //  draw left move hint cell to blue
                                temp.x = x - 1;
                                temp.y = y - 1;
                                move_hint.push_back(temp);
                                images[temp.x][temp.y].set(moveCircle);
                            }
                        }

                        if (x < 7 && y)                 //  moving right
                        {
                            if (whites[y - 1][x + 1] == false && blacks[y - 1][x + 1] == false)
                            {
                                coordinates temp;       //  draw right move hint cell to blue
                                temp.x = x + 1;
                                temp.y = y - 1;
                                move_hint.push_back(temp);
                                images[temp.x][temp.y].set(moveCircle);
                            }
                        }
                    }
                }
            }
        }
        else if (selected)                              //  click on non-black stone cell when selected
        {
            unsigned long len = move_hint.size();

            for (unsigned long i = 0; i < len; i++)
            {                                           //  cell is in movement hints
                if (move_hint[i].x == x && move_hint[i].y == y)
                {
                    turn_started = true;                //  selecting another stone is disabled until actual turn will not end

                    images[select.x][select.y].set(blockBrown);

                    if (queens[select.y][select.x])
                    {
                        images[x][y].set(queenBlack);   //  refresh stones positions
                        queens[select.y][select.x] = false;
                        queens[y][x] = true;
                    }
                    else
                    {
                        if (y == 0)                     //  transform soldier to queen
                        {
                            queens[y][x] = true;
                            images[x][y].set(queenBlack);   //  set queen icon
                        }
                        else                            //  still soldier
                            images[x][y].set(stoneBlack);   //  refresh stones positions
                    }

                    blacks[select.y][select.x] = false;
                    blacks[y][x] = true;                //  refresh stones positions

                    reset_board_color(select.x, select.y);  //  clear green selection cell

                    do                                  //  cancel actual move hint cells
                    {
                        reset_board_color(move_hint.front().x, move_hint.front().y);
                        move_hint.pop_front();
                    }while (move_hint.size());

                    if (only_jumping)                   //  remove white stone if jumping
                    {
                        int diff_x = x - select.x;      //  length between cells
                        int diff_y = y - select.y;

                        if (diff_x > 0)                 //  count cells domain differention
                            diff_x = 1;
                        else
                            diff_x = -1;

                        if (diff_y > 0)                 //  count cells relace differention
                            diff_y = 1;
                        else
                            diff_y = -1;

                        while (true)                    //  loop will be stopped manually
                        {
                            select.x += diff_x;         //  iteratively find white stone to remove
                            select.y += diff_y;

                            if (whites[select.y][select.x])
                            {
                                coordinates temp;       //  delete stone after complete jump
                                temp.x = select.x;      //  jump can be multiple
                                temp.y = select.y;
                                stones_to_remove.push_back(temp);   //  push to list

                                whites[select.y][select.x] = false;     //  refresh whites array
                                blacks[select.y][select.x] = true;  //  disable overjumping of stone by this

                                break;                  //  if stone is queen then it will be removed after
                            }
                        }
                    }

                    if (only_jumping == 1)              //  soldier multiple jump checking
                    {
                        if (x > 1 && y > 1)
                        {
                            if (whites[y - 1][x - 1] && whites[y - 2][x - 2] == false && blacks[y - 2][x - 2] == false)
                            {
                                coordinates temp;       //  draw left move hint cell to red
                                temp.x = x - 2;
                                temp.y = y - 2;
                                move_hint.push_back(temp);
                            }
                        }

                        if (x < 6 && y > 1)
                        {
                            if (whites[y - 1][x + 1] && whites[y - 2][x + 2] == false && blacks[y - 2][x + 2] == false)
                            {
                                coordinates temp;       //  draw right move hint cell to red
                                temp.x = x + 2;
                                temp.y = y - 2;
                                move_hint.push_back(temp);
                            }
                        }

                        unsigned long len = move_hint.size();

                        if (len)                        //  multiple jump is possible
                        {
                            select.x = x;               //  save coordinates
                            select.y = y;

                            if (queens[y][x])
                                images[x][y].set(queenBlackSelect);    //  set new cell with black stone as selected
                            else
                                images[x][y].set(stoneBlackSelect);    //  set new cell with black queen as selected

                            for (unsigned long i = 0; i < len; i++)   //  draw all red move hint cells
                                images[move_hint[i].x][move_hint[i].y].set(jumpCircle);

                            return;                     //  black player is still playing
                        }
                    }
                    else if (only_jumping == 2)         //  queen multiple jump checking
                    {
                        int old_x = x, old_y = y;

                        while (x > 1 && y > 1)          //  prepare for jumping left up
                        {
                            if (whites[y - 1][x - 1] && whites[y - 2][x - 2] == false &&
                                blacks[y - 2][x - 2] == false)
                            {
                                x -= 2;
                                y -= 2;

                                coordinates temp;       //  draw possible movement cell to red
                                temp.x = x;
                                temp.y = y;
                                move_hint.push_back(temp);

                                while (x && y)          //  draw all possible jump cells to red
                                {
                                    if (whites[y - 1][x - 1] || blacks[y - 1][x - 1])
                                        break;

                                    temp.x = --x;       //  draw possible movement cell to red
                                    temp.y = --y;
                                    move_hint.push_back(temp);
                                }

                                break;
                            }

                            if (whites[y - 1][x - 1] == false && blacks[y - 1][x - 1] == false)
                            {
                                x--;
                                y--;
                            }
                            else
                                break;
                        }

                        x = old_x;
                        y = old_y;

                        while (x < 6 && y > 1)          //  prepare for jumping right up
                        {
                            if (whites[y - 1][x + 1] && whites[y - 2][x + 2] == false &&
                                blacks[y - 2][x + 2] == false)
                            {
                                x += 2;
                                y -= 2;

                                coordinates temp;       //  draw possible movement cell to red
                                temp.x = x;
                                temp.y = y;
                                move_hint.push_back(temp);

                                while (x < 7 && y)      //  draw all possible jump cells to red
                                {
                                    if (whites[y - 1][x + 1] || blacks[y - 1][x + 1])
                                        break;

                                    temp.x = ++x;       //  draw possible movement cell to red
                                    temp.y = --y;
                                    move_hint.push_back(temp);
                                }

                                break;
                            }

                            if (whites[y - 1][x + 1] == false && blacks[y - 1][x + 1] == false)
                            {
                                x++;
                                y--;
                            }
                            else
                                break;
                        }

                        x = old_x;
                        y = old_y;

                        while (x > 1 && y < 6)          //  prepare for jumping left down
                        {
                            if (whites[y + 1][x - 1] && whites[y + 2][x - 2] == false &&
                                blacks[y + 2][x - 2] == false)
                            {
                                x -= 2;
                                y += 2;

                                coordinates temp;       //  draw possible movement cell to red
                                temp.x = x;
                                temp.y = y;
                                move_hint.push_back(temp);

                                while (x && y < 7)      //  draw all possible jump cells to red
                                {
                                    if (whites[y + 1][x - 1] || blacks[y + 1][x - 1])
                                        break;

                                    temp.x = --x;       //  draw possible movement cell to red
                                    temp.y = ++y;
                                    move_hint.push_back(temp);
                                }

                                break;
                            }

                            if (whites[y + 1][x - 1] == false && blacks[y + 1][x - 1] == false)
                            {
                                x--;
                                y++;
                            }
                            else
                                break;
                        }

                        x = old_x;
                        y = old_y;

                        while (x < 6 && y < 6)          //  prepare for jumping right down
                        {
                            if (whites[y + 1][x + 1] && whites[y + 2][x + 2] == false &&
                                blacks[y + 2][x + 2] == false)
                            {
                                x += 2;
                                y += 2;

                                coordinates temp;       //  draw possible movement cell to red
                                temp.x = x;
                                temp.y = y;
                                move_hint.push_back(temp);

                                while (x < 7 && y < 7)  //  draw all possible jump cells to red
                                {
                                    if (whites[y + 1][x + 1] || blacks[y + 1][x + 1])
                                        break;

                                    temp.x = ++x;       //  draw possible movement cell to red
                                    temp.y = ++y;
                                    move_hint.push_back(temp);
                                }

                                break;
                            }

                            if (whites[y + 1][x + 1] == false && blacks[y + 1][x + 1] == false)
                            {
                                x++;
                                y++;
                            }
                            else
                                break;
                        }

                        x = old_x;
                        y = old_y;

                        unsigned long len = move_hint.size();

                        if (len)                        //  multiple jump is possible
                        {
                            select.x = x;               //  save coordinates
                            select.y = y;

                            if (queens[y][x])
                                images[x][y].set(queenBlackSelect);    //  set new cell with black stone as selected
                            else
                                images[x][y].set(stoneBlackSelect);    //  set new cell with black queen as selected

                            for (unsigned long i = 0; i < len; i++)   //  draw all red move hint cells
                                images[move_hint[i].x][move_hint[i].y].set(jumpCircle);

                            return;                     //  black player is still playing
                        }
                    }

                    while (stones_to_remove.size())     //  remove all overjumped opponents stones
                    {
                        int temp_x = stones_to_remove.front().x;    //  save temporary coordinates
                        int temp_y = stones_to_remove.front().y;

                        images[temp_x][temp_y].set(blockBrown);
                        blacks[temp_y][temp_x] = false;

                        if (queens[temp_y][temp_x])
                            queens[temp_y][temp_x] = false;     //  physically remove queen

                        stones_to_remove.pop_front();   //  now remove all overjumped stones physically
                    }

                    selected = false;                   //  remove selection flag
                    white_playing = true;               //  white player will be on turn
                    turn_started = false;               //  selecting another stone is enabled from this moment

                    jump_predicate();                   //  predicate jump situations

                    return;
                }
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

    click_reaction(y, x);

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
            reset_board_color(i, j);
    }

    width -= trail;
    height -= trail;

    resize(width, height);

    return false;
}
