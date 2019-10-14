#include "checkers.h"

Checkers::Checkers()
{
    borderWidth = 2;                                            //  setup default window dimensions and border size
    width = 920;
    height = 920;
    blockWidth = width / 8;
    width = width + 2 * borderWidth;
    height = height + 2 * borderWidth;

    // headerBar.set_title("Checkers");
    // headerBar.set_has_subtitle(false);
    // headerBar.set_size_request(-1, 50);

    set_title("Checkers");                                      //  set appropriate attributes of the window object
    set_default_size(width, height);
    set_border_width(borderWidth);
    override_background_color(Gdk::RGBA("black"), Gtk::STATE_FLAG_NORMAL);
    set_icon_from_file("./resources/app_icon.png");

    // set_titlebar(headerBar);

    add(scrolledWindow);                                        //  set scrolled window as main widget of the window

    add_events(Gdk::STRUCTURE_MASK);
    signal_configure_event().connect(sigc::mem_fun(*this, &Checkers::onConfigureChanged), false);
                                                                //  load appropriate images into the pixel buffer objects
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
                                                                //  set default value of every pixel buffer
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

    initArrays();                                               //  initialize game board to the default state

    for (int i = 0; i < blockCount; i++)                        //  iterate through all game board blocks
    {
        for (int j = 0; j < blockCount; j++)
        {
            string message = to_string(i) + to_string(j);       //  get current coordinates of block

            resetBoardColor(i, j);                              //  use appropriate image for current position

            eventBoxes[i][j].add(images[i][j]);
            eventBoxes[i][j].set_events(Gdk::BUTTON_PRESS_MASK);
            eventBoxes[i][j].signal_button_press_event().connect(sigc::bind<Glib::ustring>(sigc::mem_fun(*this, &Checkers::onEventboxButtonPress), message));


            myGrid.attach(eventBoxes[i][j], i, j, 1, 1);        //  add next image into the grid object
        }
    }

    myGrid.set_halign(Gtk::ALIGN_CENTER);
    myGrid.set_valign(Gtk::ALIGN_CENTER);
    scrolledWindow.add(myGrid);                                 //  add grid into the scrolled window to allow window shrink

    show_all_children();                                        //  show all child widgets of window
}

Checkers::~Checkers()
{
}

void Checkers::initArrays()
{
    selected = false;                                           //  no cell is selected
    whitePlaying = true;                                        //  first player is on turn at start of game
    onlyJumping = 0;                                            //  insane to jump at the beginning of game
    turnStarted = false;                                        //  multiple jump is not possible at the beginning of game

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
            whites[i][j] = blacks[i][j] = queens[i][j] = false;     //  clear all description arrays
    }

    whites[0][1] = true;                                        //  set default positions of all white stones
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

    blacks[5][0] = true;                                        //  set default positions of all black stones
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

    gameOver = false;                                           //  game is not over by default
}

void Checkers::resetBoardColor(int & x, int & y)
{
    if (whites[y][x])                                           //  white stone have to be on this cell
    {
        if (queens[y][x])                                       //  queen
            images[x][y].set(queenWhite);                       //  load white queen image on this cell
        else                                                    //  stone
            images[x][y].set(stoneWhite);                       //  load white stone image on this cell
    }
    else if (blacks[y][x])                                      //  black stone have to be on this cell
    {
        if (queens[y][x])                                       //  queen
            images[x][y].set(queenBlack);                       //  load black queen image onto this cell
        else                                                    //  stone
            images[x][y].set(stoneBlack);                       //  load black stone image onto this cell
    }
    else                                                        //  this cell have to be empty
    {
        if (x & 1)                                              //  check if index value is odd or even
        {
            if (y & 1)                                          //  check if index value is odd or even
                images[x][y].set(blockWhite);                   //  use empty white block image
            else
                images[x][y].set(blockBrown);                   //  use empty black block image
        }
        else
        {
            if (y & 1)                                          //  check if index value is odd or even
                images[x][y].set(blockBrown);                   //  use empty black block image
            else
                images[x][y].set(blockWhite);                   //  use empty white block image
        }
    }
}

void Checkers::jumpPredicate()
{
    onlyJumping = 0;                                            //  set default jumping flag value

    if (whitePlaying)
    {
        for (int i = 0; i < 8; i++)                             //  jump check of white queens
        {
            for (int j = 0; j < 8; j++)
            {
                if (whites[i][j] && queens[i][j])
                {
                    int x = j, y = i;

                    while (x > 1 && y > 1)                      //  check jumping left up
                    {
                        if (blacks[y - 1][x - 1] && blacks[y - 2][x - 2] == false &&
                            whites[y - 2][x - 2] == false)
                        {
                            onlyJumping = 2;                    //  set queen jumping flag
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

                    while (x < 6 && y > 1)                      //  check jumping right up
                    {
                        if (blacks[y - 1][x + 1] && blacks[y - 2][x + 2] == false &&
                            whites[y - 2][x + 2] == false)
                        {
                            onlyJumping = 2;                    //  set queen jumping flag
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

                    while (x > 1 && y < 6)                      //  check jumping left down
                    {
                        if (blacks[y + 1][x - 1] && blacks[y + 2][x - 2] == false &&
                            whites[y + 2][x - 2] == false)
                        {
                            onlyJumping = 2;                    //  set queen jumping flag
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

                    while (x < 6 && y < 6)                      //  check jumping right down
                    {
                        if (blacks[y + 1][x + 1] && blacks[y + 2][x + 2] == false &&
                            whites[y + 2][x + 2] == false)
                        {
                            onlyJumping = 2;                    //  set queen jumping flag
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

        for (int i = 0; i < 6; i++)                             //  jump check of white soldiers
        {
            for (int j = 0; j < 8; j++)
            {
                if (whites[i][j] && queens[i][j] == false)
                {
                    if (j > 1)                                  //  check jumping left
                    {
                        if (blacks[i + 1][j - 1] == true && blacks[i + 2][j - 2] == false &&
                            whites[i + 2][j - 2] == false)
                        {
                            onlyJumping = 1;                    //  set soldier jumping flag
                            return;
                        }
                    }

                    if (j < 6)                                  //  check jumping left
                    {
                        if (blacks[i + 1][j + 1] == true && blacks[i + 2][j + 2] == false &&
                            whites[i + 2][j + 2] == false)
                        {
                            onlyJumping = 1;                    //  set soldier jumping flag
                            return;
                        }
                    }
                }
            }
        }
    }
    else                                                        //  jump check of black stones
    {
        for (int i = 0; i < 8; i++)                             //  jump check of black queens
        {
            for (int j = 0; j < 8; j++)
            {
                if (blacks[i][j] && queens[i][j])
                {
                    int x = j, y = i;

                    while (x > 1 && y > 1)                      //  check jumping left up
                    {
                        if (whites[y - 1][x - 1] && whites[y - 2][x - 2] == false &&
                            blacks[y - 2][x - 2] == false)
                        {
                            onlyJumping = 2;                    //  set queen jumping flag
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

                    while (x < 6 && y > 1)                      //  check jumping right up
                    {
                        if (whites[y - 1][x + 1] && whites[y - 2][x + 2] == false &&
                            blacks[y - 2][x + 2] == false)
                        {
                            onlyJumping = 2;                    //  set queen jumping flag
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

                    while (x > 1 && y < 6)                      //  check jumping left down
                    {
                        if (whites[y + 1][x - 1] && whites[y + 2][x - 2] == false &&
                            blacks[y + 2][x - 2] == false)
                        {
                            onlyJumping = 2;                    //  set queen jumping flag
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

                    while (x < 6 && y < 6)                      //  check jumping right down
                    {
                        if (whites[y + 1][x + 1] && whites[y + 2][x + 2] == false &&
                            blacks[y + 2][x + 2] == false)
                        {
                            onlyJumping = 2;                    //  set queen jumping flag
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

        for (int i = 2; i < 8; i++)                             //  jump check of black soldiers
        {
            for (int j = 0; j < 8; j++)
            {
                if (blacks[i][j] && queens[i][j] == false)
                {
                    if (j > 1)                                  //  check jumping left
                    {
                        if (whites[i - 1][j - 1] && whites[i - 2][j - 2] == false &&
                            blacks[i - 2][j - 2] == false)
                        {
                            onlyJumping = 1;                    //  set soldier jumping flag
                            return;
                        }
                    }

                    if (j < 6)                                  //  check jumping right
                    {
                        if (whites[i - 1][j + 1] && whites[i - 2][j + 2] == false &&
                            blacks[i - 2][j + 2] == false)
                        {
                            onlyJumping = 1;                    //  set soldier jumping flag
                            return;
                        }
                    }
                }
            }
        }
    }
}

bool Checkers::isGameOver()
{
    if (onlyJumping == 0)                                       //  jump predicate is not active
    {
        if (whitePlaying)                                       //  check game over of white player
        {
            for (int i = 0; i < 8; i++)
            {
                for (int j = 0; j < 8; j++)
                {
                    if (whites[i][j])
                    {
                        if (j && i < 7)                         //  check possible queen or soldier move left up
                        {
                            if (blacks[i + 1][j - 1] == false && whites[i + 1][j - 1] == false)
                                return false;
                        }

                        if (j < 7 && i < 7)                     //  check possible queen or soldier move right up
                        {
                            if (blacks[i + 1][j + 1] == false && whites[i + 1][j + 1] == false)
                                return false;
                        }

                        if (queens[i][j])
                        {
                            if (j && i)                         //  check possible queen move left up
                            {
                                if (blacks[i - 1][j - 1] == false && whites[i - 1][j - 1] == false)
                                    return false;
                            }

                            if (j < 7 && i)                     //  check possible queen move right up
                            {
                                if (blacks[i - 1][j + 1] == false && whites[i - 1][j + 1] == false)
                                    return false;
                            }
                        }
                    }
                }
            }
        }
        else                                                    //  check game over of black player
        {
            for (int i = 0; i < 8; i++)
            {
                for (int j = 0; j < 8; j++)
                {
                    if (blacks[i][j])
                    {
                        if (j && i)                             //  check possible queen or soldier move left up
                        {
                            if (blacks[i - 1][j - 1] == false && whites[i - 1][j - 1] == false)
                                return false;
                        }

                        if (j < 7 && i)                         //  check possible queen or soldier move right up
                        {
                            if (blacks[i - 1][j + 1] == false && whites[i - 1][j + 1] == false)
                                return false;
                        }

                        if (queens[i][j])
                        {
                            if (j && i < 7)                     //  check possible queen move left down
                            {
                                if (blacks[i + 1][j - 1] == false && whites[i + 1][j - 1] == false)
                                    return false;
                            }

                            if (j < 7 && i < 7)                 //  check possible queen move right down
                            {
                                if (blacks[i + 1][j + 1] == false && whites[i + 1][j + 1] == false)
                                    return false;
                            }
                        }
                    }
                }
            }
        }

        gameOver = true;
        return true;                                            //   end game state detected
    }

    return false;
}

void Checkers::clickReaction(int y, int x)
{
    if (whitePlaying)                                           //  white player is on turn
    {
        if (whites[y][x] && turnStarted == false)               //  it has been clicked on the white stone and there is no running multiple jump
        {
            if (queens[y][x])                                   //  there is white queen on the selected cell
            {
                bool flag = false;

                if (onlyJumping == 2)                           //  jump with white queen is possible
                {
                    int old_x = x, old_y = y;

                    while (x > 1 && y > 1)                      //  check jumping left up from actual cell
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
                        while (x < 6 && y > 1)                  //  check jumping right up from actual cell
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
                            while (x > 1 && y < 6)              //  check jumping left down from actual cell
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
                                while (x < 6 && y < 6)          //  check jumping right down from actual cell
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

                                x = old_x;                      //  reset default x and y values
                                y = old_y;
                            }

                        }
                    }
                }

                if (onlyJumping == 0)                           //  jumping with queen is not possible
                {
                    if (x && y)                                 //  check moving left up with queen
                    {
                        if (blacks[y - 1][x - 1] == false && whites[y - 1][x - 1] == false)
                            flag = true;
                    }

                    if (flag == false && x < 7 && y)            //  check moving right up with queen
                    {
                        if (blacks[y - 1][x + 1] == false && whites[y - 1][x + 1] == false)
                            flag = true;
                    }

                    if (flag == false && x && y < 7)            //  check moving left down with queen
                    {
                        if (blacks[y + 1][x - 1] == false && whites[y + 1][x - 1] == false)
                            flag = true;
                    }

                    if (flag == false && x < 7 && y < 7)        //  check moving right down with queen
                    {
                        if (blacks[y + 1][x + 1] == false && whites[y + 1][x + 1] == false)
                            flag = true;
                    }
                }

                if (flag)
                {
                    if (selected)                               //  cancel actual green selected cell
                    {
                        resetBoardColor(select.x, select.y);

                        do                                      //  cancel actual move hint cells
                        {
                            resetBoardColor(moveHint.front().x, moveHint.front().y);
                            moveHint.pop_front();
                        }while (moveHint.size());
                    }
                    else
                        selected = true;                        //  set flag if white stone was not selected

                    select.x = x;                               //  save coordinates
                    select.y = y;

                    if (queens[y][x])
                        images[x][y].set(queenWhiteSelect);     //  set new cell with white stone as selected
                    else
                        images[x][y].set(stoneWhiteSelect);     //  set new cell with white queen as selected

                    if (onlyJumping)                            //  jumping with queen is possible
                    {
                        int old_x = x, old_y = y;

                        while (x > 1 && y > 1)                  //  prepare for jumping left up
                        {
                            if (blacks[y - 1][x - 1] && blacks[y - 2][x - 2] == false &&
                                whites[y - 2][x - 2] == false)
                            {
                                x -= 2;
                                y -= 2;

                                Coordinates temp;               //  draw possible movement cell to red
                                temp.x = x;
                                temp.y = y;
                                moveHint.push_back(temp);
                                images[x][y].set(jumpCircle);

                                while (x && y)                  //  draw all possible jump cells to red
                                {
                                    if (blacks[y - 1][x - 1] || whites[y - 1][x - 1])
                                        break;

                                    temp.x = --x;               //  draw possible movement cell to red
                                    temp.y = --y;
                                    moveHint.push_back(temp);
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

                        while (x < 6 && y > 1)                  //  prepare for jumping right up
                        {
                            if (blacks[y - 1][x + 1] && blacks[y - 2][x + 2] == false &&
                                whites[y - 2][x + 2] == false)
                            {
                                x += 2;
                                y -= 2;

                                Coordinates temp;               //  draw possible movement cell to red
                                temp.x = x;
                                temp.y = y;
                                moveHint.push_back(temp);
                                images[x][y].set(jumpCircle);

                                while (x < 7 && y)              //  draw all possible jump cells to red
                                {
                                    if (blacks[y - 1][x + 1] || whites[y - 1][x + 1])
                                        break;

                                    temp.x = ++x;               //  draw possible movement cell to red
                                    temp.y = --y;
                                    moveHint.push_back(temp);
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

                        while (x > 1 && y < 6)                  //  prepare for jumping left down
                        {
                            if (blacks[y + 1][x - 1] && blacks[y + 2][x - 2] == false &&
                                whites[y + 2][x - 2] == false)
                            {
                                x -= 2;
                                y += 2;

                                Coordinates temp;               //  draw possible movement cell to red
                                temp.x = x;
                                temp.y = y;
                                moveHint.push_back(temp);
                                images[x][y].set(jumpCircle);

                                while (x && y < 7)              //  draw all possible jump cells to red
                                {
                                    if (blacks[y + 1][x - 1] || whites[y + 1][x - 1])
                                        break;

                                    temp.x = --x;               //  draw possible movement cell to red
                                    temp.y = ++y;
                                    moveHint.push_back(temp);
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

                        while (x < 6 && y < 6)                  //  prepare for jumping right down
                        {
                            if (blacks[y + 1][x + 1] && blacks[y + 2][x + 2] == false &&
                                whites[y + 2][x + 2] == false)
                            {
                                x += 2;
                                y += 2;

                                Coordinates temp;               //  draw possible movement cell to red
                                temp.x = x;
                                temp.y = y;
                                moveHint.push_back(temp);
                                images[x][y].set(jumpCircle);

                                while (x < 7 && y < 7)          //  draw all possible jump cells to red
                                {
                                    if (blacks[y + 1][x + 1] || whites[y + 1][x + 1])
                                        break;

                                    temp.x = ++x;               //  draw possible movement cell to red
                                    temp.y = ++y;
                                    moveHint.push_back(temp);
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
                    else                                        //  only moving with queen is possible
                    {
                        int old_x = x, old_y = y;

                        while (x && y)                          //  prepare for move left up
                        {
                            if (blacks[y - 1][x - 1] || whites[y - 1][x - 1])
                                break;

                            Coordinates temp;                   //  draw possible movement cell to blue
                            temp.x = --x;
                            temp.y = --y;
                            moveHint.push_back(temp);
                            images[x][y].set(moveCircle);
                        }

                        x = old_x;
                        y = old_y;

                        while (x < 7 && y)                      //  prepare for move right up
                        {
                            if (blacks[y - 1][x + 1] || whites[y - 1][x + 1])
                                break;

                            Coordinates temp;                   //  draw possible movement cell to blue
                            temp.x = ++x;
                            temp.y = --y;
                            moveHint.push_back(temp);
                            images[x][y].set(moveCircle);
                        }

                        x = old_x;
                        y = old_y;

                        while (x && y < 7)                      //  prepare for move left down
                        {
                            if (blacks[y + 1][x - 1] || whites[y + 1][x - 1])
                                break;

                            Coordinates temp;                   //  draw possible movement cell to blue
                            temp.x = --x;
                            temp.y = ++y;
                            moveHint.push_back(temp);
                            images[x][y].set(moveCircle);
                        }

                        x = old_x;
                        y = old_y;

                        while (x < 7 && y < 7)                  //  prepare for move right down
                        {
                            if (blacks[y + 1][x + 1] || whites[y + 1][x + 1])
                                break;

                            Coordinates temp;                   //  draw possible movement cell to blue
                            temp.x = ++x;
                            temp.y = ++y;
                            moveHint.push_back(temp);
                            images[x][y].set(moveCircle);
                        }
                    }
                }
            }
            else                                                //  there is white soldier on selected cell
            {
                bool flag = false;                              //  tell if white stone can move from selected cell

                if (onlyJumping == 1)
                {
                    if (x > 1 && y < 6)                         //  jumping left check
                    {
                        if (blacks[y + 1][x - 1] && blacks[y + 2][x - 2] == false && whites[y + 2][x - 2] == false)
                            flag = true;
                    }

                    if (flag == false && x < 6 && y < 6)        //  jumping right check
                    {
                        if (blacks[y + 1][x + 1] && blacks[y + 2][x + 2] == false && whites[y + 2][x + 2] == false)
                            flag = true;
                    }
                }

                if (onlyJumping == 0)                           //  stone cannot move when jumping is possible
                {
                    if (x && y < 7)                             //  moving left check
                    {
                        if (blacks[y + 1][x - 1] == false && whites[y + 1][x - 1] == false)
                            flag = true;
                    }

                    if (flag == false && x < 7 && y < 7)        //  moving right check
                    {
                        if (blacks[y + 1][x + 1] == false && whites[y + 1][x + 1] == false)
                            flag = true;
                    }
                }

                if (flag)                                       //  move or jump is possible from selected cell
                {
                    if (selected)                               //  cancel actual green selected cell
                    {
                        resetBoardColor(select.x, select.y);

                        do                                      //  cancel actual move hint cells
                        {
                            resetBoardColor(moveHint.front().x, moveHint.front().y);
                            moveHint.pop_front();
                        }while (moveHint.size());
                    }
                    else
                        selected = true;                        //  set flag if white stone was not selected

                    select.x = x;                               //  save coordinates
                    select.y = y;

                    if (queens[y][x])
                        images[x][y].set(queenWhiteSelect);     //  set new cell with white stone as selected
                    else
                        images[x][y].set(stoneWhiteSelect);     //  set new cell with white queen as selected

                    if (onlyJumping)                            //  jumping is required when it is possible to jump
                    {
                        if (x > 1 && y < 6)                     //  jumping left
                        {
                            if (blacks[y + 1][x - 1] && blacks[y + 2][x - 2] == false && whites[y + 2][x - 2] == false)
                            {
                                Coordinates temp;               //  draw left move hint cell to red
                                temp.x = x - 2;
                                temp.y = y + 2;
                                moveHint.push_back(temp);

                                images[temp.x][temp.y].set(jumpCircle);
                            }
                        }

                        if (x < 6 && y < 6)                     //  jumping right
                        {
                            if (blacks[y + 1][x + 1] && blacks[y + 2][x + 2] == false && whites[y + 2][x + 2] == false)
                            {
                                Coordinates temp;               //  draw right move hint cell to red
                                temp.x = x + 2;
                                temp.y = y + 2;
                                moveHint.push_back(temp);

                                images[temp.x][temp.y].set(jumpCircle);
                            }
                        }
                    }
                    else                                        //  stone cannot move when jumping is possible
                    {
                        if (x && y < 7)                         //  moving left
                        {
                            if (blacks[y + 1][x - 1] == false && whites[y + 1][x - 1] == false)
                            {
                                Coordinates temp;               //  draw left move hint cell to blue
                                temp.x = x - 1;
                                temp.y = y + 1;
                                moveHint.push_back(temp);
                                images[temp.x][temp.y].set(moveCircle);
                            }
                        }

                        if (x < 7 && y < 7)                     //  moving right
                        {
                            if (blacks[y + 1][x + 1] == false && whites[y + 1][x + 1] == false)
                            {
                                Coordinates temp;               //  draw right move hint cell to blue
                                temp.x = x + 1;
                                temp.y = y + 1;
                                moveHint.push_back(temp);
                                images[temp.x][temp.y].set(moveCircle);
                            }
                        }
                    }
                }
            }
        }
        else if (selected)                                      //  click on non-white stone cell when selected
        {
            unsigned long len = moveHint.size();

            for (unsigned long i = 0; i < len; i++)
            {                                                   //  cell is in movement hints
                if (moveHint[i].x == x && moveHint[i].y == y)
                {
                    turnStarted = true;                         //  selecting another stone is disabled until actual turn will not end

                    images[select.x][select.y].set(blockBrown);

                    if (queens[select.y][select.x])
                    {
                        images[x][y].set(queenWhite);           //  refresh stones positions
                        queens[select.y][select.x] = false;
                        queens[y][x] = true;
                    }
                    else
                    {
                        if (y == 7)                             //  transform soldier to queen
                        {
                            queens[y][x] = true;
                            images[x][y].set(queenWhite);       //  set queen icon
                        }
                        else                                    //  still soldier
                            images[x][y].set(stoneWhite);       //  refresh stones positions
                    }

                    whites[select.y][select.x] = false;
                    whites[y][x] = true;                        //  refresh stones positions

                    resetBoardColor(select.x, select.y);        //  clear green selection cell

                    do                                          //  cancel actual move hint cells
                    {
                        resetBoardColor(moveHint.front().x, moveHint.front().y);
                        moveHint.pop_front();
                    }while (moveHint.size());

                    if (onlyJumping)                            //  remove black stone if jumping
                    {
                        int diff_x = x - select.x;              //  length between cells
                        int diff_y = y - select.y;

                        if (diff_x > 0)                         //  count cells domain differention
                            diff_x = 1;
                        else
                            diff_x = -1;

                        if (diff_y > 0)                         //  count cells relace differention
                            diff_y = 1;
                        else
                            diff_y = -1;

                        while (true)                            //  loop will be stopped manually
                        {
                            select.x += diff_x;                 //  iteratively find white stone to remove
                            select.y += diff_y;

                            if (blacks[select.y][select.x])
                            {
                                Coordinates temp;               //  delete stone after complete jump
                                temp.x = select.x;              //  jump can be multiple
                                temp.y = select.y;
                                stonesToRemove.push_back(temp);     //  push to list

                                blacks[select.y][select.x] = false;     //  refresh blacks array
                                whites[select.y][select.x] = true;      //  disable overjumping of stone by this

                                break;                          //  if stone is queen then it will be removed after
                            }
                        }
                    }

                    if (onlyJumping == 1)                       //  soldier multiple jump checking
                    {
                        if (x > 1 && y < 6)
                        {
                            if (blacks[y + 1][x - 1] && blacks[y + 2][x - 2] == false && whites[y + 2][x - 2] == false)
                            {
                                Coordinates temp;               //  draw left move hint cell to red
                                temp.x = x - 2;
                                temp.y = y + 2;
                                moveHint.push_back(temp);
                            }
                        }

                        if (x < 6 && y < 6)
                        {
                            if (blacks[y + 1][x + 1] && blacks[y + 2][x + 2] == false && whites[y + 2][x + 2] == false)
                            {
                                Coordinates temp;               //  draw right move hint cell to red
                                temp.x = x + 2;
                                temp.y = y + 2;
                                moveHint.push_back(temp);
                            }
                        }

                        unsigned long len = moveHint.size();

                        if (len)                                //  multiple jump is possible
                        {
                            select.x = x;                       //  save coordinates
                            select.y = y;

                            if (queens[y][x])
                                images[x][y].set(queenWhiteSelect);     //  set new cell with white stone as selected
                            else
                                images[x][y].set(stoneWhiteSelect);     //  set new cell with white queen as selected

                            for (unsigned long i = 0; i < len; i++)     //  draw all red move hint cells
                                images[moveHint[i].x][moveHint[i].y].set(jumpCircle);

                            return;                             //  white player is still playing
                        }
                    }
                    else if (onlyJumping == 2)                  //  queen multiple jump checking
                    {
                        int old_x = x, old_y = y;

                        while (x > 1 && y > 1)                  //  prepare for jumping left up
                        {
                            if (blacks[y - 1][x - 1] && blacks[y - 2][x - 2] == false &&
                                whites[y - 2][x - 2] == false)
                            {
                                x -= 2;
                                y -= 2;

                                Coordinates temp;               //  draw possible movement cell to red
                                temp.x = x;
                                temp.y = y;
                                moveHint.push_back(temp);

                                while (x && y)                  //  draw all possible jump cells to red
                                {
                                    if (blacks[y - 1][x - 1] || whites[y - 1][x - 1])
                                        break;

                                    temp.x = --x;               //  draw possible movement cell to red
                                    temp.y = --y;
                                    moveHint.push_back(temp);
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

                        while (x < 6 && y > 1)                  //  prepare for jumping right up
                        {
                            if (blacks[y - 1][x + 1] && blacks[y - 2][x + 2] == false &&
                                whites[y - 2][x + 2] == false)
                            {
                                x += 2;
                                y -= 2;

                                Coordinates temp;               //  draw possible movement cell to red
                                temp.x = x;
                                temp.y = y;
                                moveHint.push_back(temp);

                                while (x < 7 && y)              //  draw all possible jump cells to red
                                {
                                    if (blacks[y - 1][x + 1] || whites[y - 1][x + 1])
                                        break;

                                    temp.x = ++x;               //  draw possible movement cell to red
                                    temp.y = --y;
                                    moveHint.push_back(temp);
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

                        while (x > 1 && y < 6)                  //  prepare for jumping left down
                        {
                            if (blacks[y + 1][x - 1] && blacks[y + 2][x - 2] == false &&
                                whites[y + 2][x - 2] == false)
                            {
                                x -= 2;
                                y += 2;

                                Coordinates temp;               //  draw possible movement cell to red
                                temp.x = x;
                                temp.y = y;
                                moveHint.push_back(temp);

                                while (x && y < 7)              //  draw all possible jump cells to red
                                {
                                    if (blacks[y + 1][x - 1] || whites[y + 1][x - 1])
                                        break;

                                    temp.x = --x;               //  draw possible movement cell to red
                                    temp.y = ++y;
                                    moveHint.push_back(temp);
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

                        while (x < 6 && y < 6)                  //  prepare for jumping right down
                        {
                            if (blacks[y + 1][x + 1] && blacks[y + 2][x + 2] == false &&
                                whites[y + 2][x + 2] == false)
                            {
                                x += 2;
                                y += 2;

                                Coordinates temp;               //  draw possible movement cell to red
                                temp.x = x;
                                temp.y = y;
                                moveHint.push_back(temp);

                                while (x < 7 && y < 7)          //  draw all possible jump cells to red
                                {
                                    if (blacks[y + 1][x + 1] || whites[y + 1][x + 1])
                                        break;

                                    temp.x = ++x;               //  draw possible movement cell to red
                                    temp.y = ++y;
                                    moveHint.push_back(temp);
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

                        unsigned long len = moveHint.size();

                        if (len)                                //  multiple jump is possible
                        {
                            select.x = x;                       //  save coordinates
                            select.y = y;

                            if (queens[y][x])
                                images[x][y].set(queenWhiteSelect);     //  set new cell with white stone as selected
                            else
                                images[x][y].set(stoneWhiteSelect);     //  set new cell with white queen as selected

                            for (unsigned long i = 0; i < len; i++)     //  draw all red move hint cells
                                images[moveHint[i].x][moveHint[i].y].set(jumpCircle);

                            return;                             //  white player is still playing
                        }
                    }

                    while (stonesToRemove.size())               //  remove all overjumped opponents stones
                    {
                        int temp_x = stonesToRemove.front().x;  //  save temporary coordinates
                        int temp_y = stonesToRemove.front().y;

                        images[temp_x][temp_y].set(blockBrown);
                        whites[temp_y][temp_x] = false;

                        if (queens[temp_y][temp_x])
                            queens[temp_y][temp_x] = false;     //  physically remove queen

                        stonesToRemove.pop_front();             //  now remove all overjumped stones physically
                    }

                    selected = false;                           //  remove selection flag
                    whitePlaying = false;                       //  black player will be on turn
                    turnStarted = false;                        //  selecting another stone is enabled from this moment

                    jumpPredicate();                            //  predicate jump situations

                    return;
                }
            }
        }
    }
    else                                                        //  black player is on turn
    {
        if (blacks[y][x] && turnStarted == false)               //  it was clicked on black stone and there is no running multiple jump
        {
            if (queens[y][x])                                   //  there is black queen on selected cell
            {
                bool flag = false;

                if (onlyJumping == 2)                           //  jump with black queen is possible
                {
                    int old_x = x, old_y = y;

                    while (x > 1 && y > 1)                      //  check jumping left up from actual cell
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
                        while (x < 6 && y > 1)                  //  check jumping right up from actual cell
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
                            while (x > 1 && y < 6)              //  check jumping left down from actual cell
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
                                while (x < 6 && y < 6)          //  check jumping right down from actual cell
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

                                x = old_x;                      //  reset default x and y values
                                y = old_y;
                            }
                        }
                    }
                }

                if (onlyJumping == 0)                           //  jumping with queen is not possible
                {
                    if (x && y)                                 //  check moving left up with queen
                    {
                        if (whites[y - 1][x - 1] == false && blacks[y - 1][x - 1] == false)
                            flag = true;
                    }

                    if (flag == false && x < 7 && y)            //  check moving right up with queen
                    {
                        if (whites[y - 1][x + 1] == false && blacks[y - 1][x + 1] == false)
                            flag = true;
                    }

                    if (flag == false && x && y < 7)            //  check moving left down with queen
                    {
                        if (whites[y + 1][x - 1] == false && blacks[y + 1][x - 1] == false)
                            flag = true;
                    }

                    if (flag == false && x < 7 && y < 7)        //  check moving right down with queen
                    {
                        if (whites[y + 1][x + 1] == false && blacks[y + 1][x + 1] == false)
                            flag = true;
                    }
                }

                if (flag)
                {
                    if (selected)                               //  cancel actual green selected cell
                    {
                        resetBoardColor(select.x, select.y);

                        do                                      //  cancel actual move hint cells
                        {
                            resetBoardColor(moveHint.front().x, moveHint.front().y);
                            moveHint.pop_front();
                        }while (moveHint.size());
                    }
                    else
                        selected = true;                        //  set flag if black stone was not selected

                    select.x = x;                               //  save coordinates
                    select.y = y;

                    if (queens[y][x])
                        images[x][y].set(queenBlackSelect);     //  set new cell with black stone as selected
                    else
                        images[x][y].set(stoneBlackSelect);     //  set new cell with black queen as selected

                    if (onlyJumping)                            //  jumping with queen is possible
                    {
                        int old_x = x, old_y = y;

                        while (x > 1 && y > 1)                  //  prepare for jumping left up
                        {
                            if (whites[y - 1][x - 1] && whites[y - 2][x - 2] == false &&
                                blacks[y - 2][x - 2] == false)
                            {
                                x -= 2;
                                y -= 2;

                                Coordinates temp;               //  draw possible movement cell to red
                                temp.x = x;
                                temp.y = y;
                                moveHint.push_back(temp);
                                images[x][y].set(jumpCircle);

                                while (x && y)                  //  draw all possible jump cells to red
                                {
                                    if (whites[y - 1][x - 1] || blacks[y - 1][x - 1])
                                        break;

                                    temp.x = --x;               //  draw possible movement cell to red
                                    temp.y = --y;
                                    moveHint.push_back(temp);
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

                        while (x < 6 && y > 1)                  //  prepare for jumping right up
                        {
                            if (whites[y - 1][x + 1] && whites[y - 2][x + 2] == false &&
                                blacks[y - 2][x + 2] == false)
                            {
                                x += 2;
                                y -= 2;

                                Coordinates temp;               //  draw possible movement cell to red
                                temp.x = x;
                                temp.y = y;
                                moveHint.push_back(temp);
                                images[x][y].set(jumpCircle);

                                while (x < 7 && y)              //  draw all possible jump cells to red
                                {
                                    if (whites[y - 1][x + 1] || blacks[y - 1][x + 1])
                                        break;

                                    temp.x = ++x;               //  draw possible movement cell to red
                                    temp.y = --y;
                                    moveHint.push_back(temp);
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

                        while (x > 1 && y < 6)                  //  prepare for jumping left down
                        {
                            if (whites[y + 1][x - 1] && whites[y + 2][x - 2] == false &&
                                blacks[y + 2][x - 2] == false)
                            {
                                x -= 2;
                                y += 2;

                                Coordinates temp;               //  draw possible movement cell to red
                                temp.x = x;
                                temp.y = y;
                                moveHint.push_back(temp);
                                images[x][y].set(jumpCircle);

                                while (x && y < 7)              //  draw all possible jump cells to red
                                {
                                    if (whites[y + 1][x - 1] || blacks[y + 1][x - 1])
                                        break;

                                    temp.x = --x;               //  draw possible movement cell to red
                                    temp.y = ++y;
                                    moveHint.push_back(temp);
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

                        while (x < 6 && y < 6)                  //  prepare for jumping right down
                        {
                            if (whites[y + 1][x + 1] && whites[y + 2][x + 2] == false &&
                                blacks[y + 2][x + 2] == false)
                            {
                                x += 2;
                                y += 2;

                                Coordinates temp;               //  draw possible movement cell to red
                                temp.x = x;
                                temp.y = y;
                                moveHint.push_back(temp);
                                images[x][y].set(jumpCircle);

                                while (x < 7 && y < 7)          //  draw all possible jump cells to red
                                {
                                    if (whites[y + 1][x + 1] || blacks[y + 1][x + 1])
                                        break;

                                    temp.x = ++x;               //  draw possible movement cell to red
                                    temp.y = ++y;
                                    moveHint.push_back(temp);
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
                    else                                        //  only moving with queen is possible
                    {
                        int old_x = x, old_y = y;

                        while (x && y)                          //  prepare for move left up
                        {
                            if (whites[y - 1][x - 1] || blacks[y - 1][x - 1])
                                break;

                            Coordinates temp;                   //  draw possible movement cell to blue
                            temp.x = --x;
                            temp.y = --y;
                            moveHint.push_back(temp);
                            images[x][y].set(moveCircle);
                        }

                        x = old_x;
                        y = old_y;

                        while (x < 7 && y)                      //  prepare for move right up
                        {
                            if (whites[y - 1][x + 1] || blacks[y - 1][x + 1])
                                break;

                            Coordinates temp;                   //  draw possible movement cell to blue
                            temp.x = ++x;
                            temp.y = --y;
                            moveHint.push_back(temp);
                            images[x][y].set(moveCircle);
                        }

                        x = old_x;
                        y = old_y;

                        while (x && y < 7)                      //  prepare for move left down
                        {
                            if (whites[y + 1][x - 1] || blacks[y + 1][x - 1])
                                break;

                            Coordinates temp;                   //  draw possible movement cell to blue
                            temp.x = --x;
                            temp.y = ++y;
                            moveHint.push_back(temp);
                            images[x][y].set(moveCircle);
                        }

                        x = old_x;
                        y = old_y;

                        while (x < 7 && y < 7)                  //  prepare for move right down
                        {
                            if (whites[y + 1][x + 1] || blacks[y + 1][x + 1])
                                break;

                            Coordinates temp;                   //  draw possible movement cell to blue
                            temp.x = ++x;
                            temp.y = ++y;
                            moveHint.push_back(temp);
                            images[x][y].set(moveCircle);
                        }
                    }
                }
            }
            else                                                //  there is black soldier on selected cell
            {
                bool flag = false;                              //  tell if black stone can move from selected cell

                if (onlyJumping == 1)
                {
                    if (x > 1 && y > 1)                         //  jumping left check
                    {
                        if (whites[y - 1][x - 1] && whites[y - 2][x - 2] == false && blacks[y - 2][x - 2] == false)
                            flag = true;
                    }

                    if (flag == false && x < 6 && y > 1)        //  jumping right check
                    {
                        if (whites[y - 1][x + 1] && whites[y - 2][x + 2] == false && blacks[y - 2][x + 2] == false)
                            flag = true;
                    }
                }

                if (onlyJumping == 0)                           //  stone cannot move when jumping is possible
                {
                    if (x && y)                                 //  moving left check
                    {
                        if (whites[y - 1][x - 1] == false && blacks[y - 1][x - 1] == false)
                            flag = true;
                    }

                    if (flag == false && x < 7 && y)            //  moving right check
                    {
                        if (whites[y - 1][x + 1] == false && blacks[y - 1][x + 1] == false)
                            flag = true;
                    }
                }

                if (flag)                                       //  move or jump is possible from selected cell
                {
                    if (selected)                               //  cancel actual green selected cell
                    {
                        resetBoardColor(select.x, select.y);

                        do                                      //  cancel actual move hint cells
                        {
                            resetBoardColor(moveHint.front().x, moveHint.front().y);
                            moveHint.pop_front();
                        }while (moveHint.size());
                    }
                    else
                        selected = true;                        //  set flag if black stone was not selected

                    select.x = x;                               //  save coordinates
                    select.y = y;

                    if (queens[y][x])
                        images[x][y].set(queenBlackSelect);     //  set new cell with black stone as selected
                    else
                        images[x][y].set(stoneBlackSelect);     //  set new cell with black queen as selected

                    if (onlyJumping)                            //  jumping is required when it is possible to jump
                    {
                        if (x > 1 && y > 1)                     //  jumping left
                        {
                            if (whites[y - 1][x - 1] && whites[y - 2][x - 2] == false && blacks[y - 2][x - 2] == false)
                            {
                                Coordinates temp;               //  draw left move hint cell to red
                                temp.x = x - 2;
                                temp.y = y - 2;
                                moveHint.push_back(temp);

                                images[temp.x][temp.y].set(jumpCircle);
                            }
                        }

                        if (x < 6 && y > 1)                     //  jumping right
                        {
                            if (whites[y - 1][x + 1] && whites[y - 2][x + 2] == false && blacks[y - 2][x + 2] == false)
                            {
                                Coordinates temp;               //  draw right move hint cell to red
                                temp.x = x + 2;
                                temp.y = y - 2;
                                moveHint.push_back(temp);

                                images[temp.x][temp.y].set(jumpCircle);
                            }
                        }
                    }
                    else                                        //  stone cannot move when jumping is possible
                    {
                        if (x && y)                             //  moving left
                        {
                            if (whites[y - 1][x - 1] == false && blacks[y - 1][x - 1] == false)
                            {
                                Coordinates temp;               //  draw left move hint cell to blue
                                temp.x = x - 1;
                                temp.y = y - 1;
                                moveHint.push_back(temp);
                                images[temp.x][temp.y].set(moveCircle);
                            }
                        }

                        if (x < 7 && y)                         //  moving right
                        {
                            if (whites[y - 1][x + 1] == false && blacks[y - 1][x + 1] == false)
                            {
                                Coordinates temp;               //  draw right move hint cell to blue
                                temp.x = x + 1;
                                temp.y = y - 1;
                                moveHint.push_back(temp);
                                images[temp.x][temp.y].set(moveCircle);
                            }
                        }
                    }
                }
            }
        }
        else if (selected)                                      //  click on non-black stone cell when selected
        {
            unsigned long len = moveHint.size();

            for (unsigned long i = 0; i < len; i++)
            {                                                   //  cell is in movement hints
                if (moveHint[i].x == x && moveHint[i].y == y)
                {
                    turnStarted = true;                         //  selecting another stone is disabled until actual turn will not end

                    images[select.x][select.y].set(blockBrown);

                    if (queens[select.y][select.x])
                    {
                        images[x][y].set(queenBlack);           //  refresh stones positions
                        queens[select.y][select.x] = false;
                        queens[y][x] = true;
                    }
                    else
                    {
                        if (y == 0)                             //  transform soldier to queen
                        {
                            queens[y][x] = true;
                            images[x][y].set(queenBlack);       //  set queen icon
                        }
                        else                                    //  still soldier
                            images[x][y].set(stoneBlack);       //  refresh stones positions
                    }

                    blacks[select.y][select.x] = false;
                    blacks[y][x] = true;                        //  refresh stones positions

                    resetBoardColor(select.x, select.y);        //  clear green selection cell

                    do                                          //  cancel actual move hint cells
                    {
                        resetBoardColor(moveHint.front().x, moveHint.front().y);
                        moveHint.pop_front();
                    }while (moveHint.size());

                    if (onlyJumping)                            //  remove white stone if jumping
                    {
                        int diff_x = x - select.x;              //  length between cells
                        int diff_y = y - select.y;

                        if (diff_x > 0)                         //  count cells domain differention
                            diff_x = 1;
                        else
                            diff_x = -1;

                        if (diff_y > 0)                         //  count cells relace differention
                            diff_y = 1;
                        else
                            diff_y = -1;

                        while (true)                            //  loop will be stopped manually
                        {
                            select.x += diff_x;                 //  iteratively find white stone to remove
                            select.y += diff_y;

                            if (whites[select.y][select.x])
                            {
                                Coordinates temp;               //  delete stone after complete jump
                                temp.x = select.x;              //  jump can be multiple
                                temp.y = select.y;
                                stonesToRemove.push_back(temp);     //  push to list

                                whites[select.y][select.x] = false;     //  refresh whites array
                                blacks[select.y][select.x] = true;      //  disable overjumping of stone by this

                                break;                          //  if stone is queen then it will be removed after
                            }
                        }
                    }

                    if (onlyJumping == 1)                       //  soldier multiple jump checking
                    {
                        if (x > 1 && y > 1)
                        {
                            if (whites[y - 1][x - 1] && whites[y - 2][x - 2] == false && blacks[y - 2][x - 2] == false)
                            {
                                Coordinates temp;               //  draw left move hint cell to red
                                temp.x = x - 2;
                                temp.y = y - 2;
                                moveHint.push_back(temp);
                            }
                        }

                        if (x < 6 && y > 1)
                        {
                            if (whites[y - 1][x + 1] && whites[y - 2][x + 2] == false && blacks[y - 2][x + 2] == false)
                            {
                                Coordinates temp;               //  draw right move hint cell to red
                                temp.x = x + 2;
                                temp.y = y - 2;
                                moveHint.push_back(temp);
                            }
                        }

                        unsigned long len = moveHint.size();

                        if (len)                                //  multiple jump is possible
                        {
                            select.x = x;                       //  save coordinates
                            select.y = y;

                            if (queens[y][x])
                                images[x][y].set(queenBlackSelect);     //  set new cell with black stone as selected
                            else
                                images[x][y].set(stoneBlackSelect);     //  set new cell with black queen as selected

                            for (unsigned long i = 0; i < len; i++)     //  draw all red move hint cells
                                images[moveHint[i].x][moveHint[i].y].set(jumpCircle);

                            return;                             //  black player is still playing
                        }
                    }
                    else if (onlyJumping == 2)                  //  queen multiple jump checking
                    {
                        int old_x = x, old_y = y;

                        while (x > 1 && y > 1)                  //  prepare for jumping left up
                        {
                            if (whites[y - 1][x - 1] && whites[y - 2][x - 2] == false &&
                                blacks[y - 2][x - 2] == false)
                            {
                                x -= 2;
                                y -= 2;

                                Coordinates temp;               //  draw possible movement cell to red
                                temp.x = x;
                                temp.y = y;
                                moveHint.push_back(temp);

                                while (x && y)                  //  draw all possible jump cells to red
                                {
                                    if (whites[y - 1][x - 1] || blacks[y - 1][x - 1])
                                        break;

                                    temp.x = --x;               //  draw possible movement cell to red
                                    temp.y = --y;
                                    moveHint.push_back(temp);
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

                        while (x < 6 && y > 1)                  //  prepare for jumping right up
                        {
                            if (whites[y - 1][x + 1] && whites[y - 2][x + 2] == false &&
                                blacks[y - 2][x + 2] == false)
                            {
                                x += 2;
                                y -= 2;

                                Coordinates temp;               //  draw possible movement cell to red
                                temp.x = x;
                                temp.y = y;
                                moveHint.push_back(temp);

                                while (x < 7 && y)              //  draw all possible jump cells to red
                                {
                                    if (whites[y - 1][x + 1] || blacks[y - 1][x + 1])
                                        break;

                                    temp.x = ++x;               //  draw possible movement cell to red
                                    temp.y = --y;
                                    moveHint.push_back(temp);
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

                        while (x > 1 && y < 6)                  //  prepare for jumping left down
                        {
                            if (whites[y + 1][x - 1] && whites[y + 2][x - 2] == false &&
                                blacks[y + 2][x - 2] == false)
                            {
                                x -= 2;
                                y += 2;

                                Coordinates temp;               //  draw possible movement cell to red
                                temp.x = x;
                                temp.y = y;
                                moveHint.push_back(temp);

                                while (x && y < 7)              //  draw all possible jump cells to red
                                {
                                    if (whites[y + 1][x - 1] || blacks[y + 1][x - 1])
                                        break;

                                    temp.x = --x;               //  draw possible movement cell to red
                                    temp.y = ++y;
                                    moveHint.push_back(temp);
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

                        while (x < 6 && y < 6)                  //  prepare for jumping right down
                        {
                            if (whites[y + 1][x + 1] && whites[y + 2][x + 2] == false &&
                                blacks[y + 2][x + 2] == false)
                            {
                                x += 2;
                                y += 2;

                                Coordinates temp;               //  draw possible movement cell to red
                                temp.x = x;
                                temp.y = y;
                                moveHint.push_back(temp);

                                while (x < 7 && y < 7)          //  draw all possible jump cells to red
                                {
                                    if (whites[y + 1][x + 1] || blacks[y + 1][x + 1])
                                        break;

                                    temp.x = ++x;               //  draw possible movement cell to red
                                    temp.y = ++y;
                                    moveHint.push_back(temp);
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

                        unsigned long len = moveHint.size();

                        if (len)                                //  multiple jump is possible
                        {
                            select.x = x;                       //  save coordinates
                            select.y = y;

                            if (queens[y][x])
                                images[x][y].set(queenBlackSelect);     //  set new cell with black stone as selected
                            else
                                images[x][y].set(stoneBlackSelect);     //  set new cell with black queen as selected

                            for (unsigned long i = 0; i < len; i++)     //  draw all red move hint cells
                                images[moveHint[i].x][moveHint[i].y].set(jumpCircle);

                            return;                             //  black player is still playing
                        }
                    }

                    while (stonesToRemove.size())               //  remove all overjumped opponents stones
                    {
                        int temp_x = stonesToRemove.front().x;  //  save temporary coordinates
                        int temp_y = stonesToRemove.front().y;

                        images[temp_x][temp_y].set(blockBrown);
                        blacks[temp_y][temp_x] = false;

                        if (queens[temp_y][temp_x])
                            queens[temp_y][temp_x] = false;     //  physically remove queen

                        stonesToRemove.pop_front();             //  now remove all overjumped stones physically
                    }

                    selected = false;                           //  remove selection flag
                    whitePlaying = true;                        //  white player will be on turn
                    turnStarted = false;                        //  selecting another stone is enabled from this moment

                    jumpPredicate();                            //  predicate jump situations

                    return;
                }
            }
        }
    }
}

bool Checkers::onEventboxButtonPress(GdkEventButton * /*button_event*/, Glib::ustring data)
{
    int x;                                                      //  coordinates of selected block
    int y;

    switch (data[0])                                            //  convert first character to the integer index
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

    switch (data[1])                                            //  convert second character to the integer index
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

    clickReaction(y, x);                                        //  handle click using appropriate method

    // hide();
    return true;
}

bool Checkers::onConfigureChanged(GdkEventConfigure * event)
{
    int newWidth = event->width;                                //  get current dimensions of the window
    int newHeight = event->height;

    bool changed = false;                                       //  holds if window size has been changed

    if (newWidth != width)                                      //  check if width value has been updated
    {
        width = newWidth;
        changed = true;
    }

    if (newHeight != height)                                    //  check if height value has been updated
    {
        height = newHeight;
        changed = true;
    }

    if (changed == false)                                       //  exit method if window dimensions are the same
        return false;

    int chunk;

    if (width < height)
        chunk = (width - 2 * borderWidth) / 8;
    else
        chunk = (height - 2 * borderWidth) / 8;

    blockBrown = blockBrownOriginal->scale_simple(chunk, chunk, Gdk::INTERP_BILINEAR);     //  scale all pixel buffers to the calculated block size
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

    for (int i = 0; i < blockCount; i++)                        //  iterate through all game board blocks
    {
        for (int j = 0; j < blockCount; j++)
            resetBoardColor(i, j);                              //  use appropriate image for current position
    }

    if (selected)                                               //  check if any stone has already been selected
    {
        int x = select.x;                                       //  get selection coordinates
        int y = select.y;

        if (queens[y][x])                                       //  queen has been selected
        {
            if (whitePlaying)                                   //  handle which player is on turn
                images[x][y].set(queenWhiteSelect);
            else
                images[x][y].set(queenBlackSelect);             //  use appropriate scaled image for selected block
        }
        else                                                    //  regular stone has been selected
        {
            if (whitePlaying)                                   //  handle which player is on turn
                images[x][y].set(stoneWhiteSelect);
            else
                images[x][y].set(stoneBlackSelect);             //  use appropriate scaled image for selected block
        }

        int N = moveHint.size();                                //  get size of all possible movement positions
        
        if (onlyJumping)                                        //  attack can be performed
        {
            for (int i = 0; i < N; i++)                         //  iterate through all movement blocks
                images[moveHint[i].x][moveHint[i].y].set(jumpCircle);   //  use appropriate scaled image for selected block
        }
        else                                                    //  attack cannot be performed
        {
            for (int i = 0; i < N; i++)                         //  iterate through all movement blocks
                images[moveHint[i].x][moveHint[i].y].set(moveCircle);   //  use appropriate scaled image for selected block
        }
    }

    return false;
}
