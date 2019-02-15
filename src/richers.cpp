#include "richers.h"
#include "ui_richers.h"

ClickableLabel::ClickableLabel(const QString & text, QWidget * parent) : QLabel(parent)
{
    setText(text);
}

ClickableLabel::~ClickableLabel()
{
}

void ClickableLabel::mousePressEvent(QMouseEvent * event)
{
    emit clicked(line, field);                          //  emit signal if mouse press event detected on clickable label
    event->ignore();
}

richers::richers(QWidget * parent) : QWidget(parent), ui(new Ui::richers)
{
    ui->setupUi(this);

    if (QApplication::desktop()->screenGeometry().width() >= 1024 && QApplication::desktop()->screenGeometry().height() >= 1024)
        high_resolution = true;                         //  it is possible to start application in high resolution(930 x 930)
    else
    {
        high_resolution = false;                        //  application can be started only in standard resolution(680 x 680)
        setMinimumSize(680, 680);
        setMaximumSize(680, 680);
        ui->about_image->setMinimumSize(680, 680);
        ui->about_image->setMaximumSize(680, 680);
        ui->help_image->setMinimumSize(680, 680);
        ui->help_image->setMaximumSize(680, 680);
    }

    create_colors();                                    //  call all initializing methods
    init_table();
    create_icons();
    init_arrays();
    init_icons();
    init_menu();

    ui->tabs->tabBar()->hide();                         //  hide tab bar of tab widget
    ui->tab->setLayout(ui->tab1_g_layout);              //  set layout for first tab of tab widget
    ui->tab_2->setLayout(ui->tab2_g_layout);            //  set layout for second tab of tab widget
    ui->tab_3->setLayout(ui->tab3_v_layout);            //  set layout for third tab of tab widget
    ui->tab_4->setLayout(ui->tab4_v_layout);            //  set layout for fourth tab of tab widget
    setLayout(ui->main_v_layout);                       //  set main layout of window
}

richers::~richers()
{
    delete ui;
}

void richers::keyPressEvent(QKeyEvent * event)
{
    if (event->key() == Qt::Key_Escape || event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter || event->key() == Qt::Key_Space)
    {
        if (workspace == game)                          //  game tab is active
            switch_to_menu();                           //  move to game tab in tab widget
        else if (workspace == menu)                     //  menu tab is active
            switch_to_game();                           //  move to game tab in tab widget
        else if (workspace == about)                    //  about tab is active
        {
            setWindowTitle("Richers");
            workspace = menu;
            ui->tabs->setCurrentIndex(menu);            //  move to menu tab in tab widget
        }
        else                                            //  help tab is active
        {
            workspace = menu;
            ui->tabs->setCurrentIndex(menu);            //  move to menu tab in tab widget
        }
    }
}

void richers::create_colors()
{
    white_palette.setColor(QPalette::Window, QColor(179, 179, 179));    //  initialize background palettes
    black_palette.setColor(QPalette::Window, Qt::black);
}

void richers::init_table()
{
    block_brown = new QPixmap(":/resources/block_brown.jpg");   //  allocate memory for all images and set path to source images
    block_white = new QPixmap(":/resources/block_white.jpg");
    stone_black = new QPixmap(":/resources/stone_brown.jpg");
    stone_white = new QPixmap(":/resources/stone_white.jpg");
    queen_black = new QPixmap(":/resources/queen_brown.jpg");
    queen_white = new QPixmap(":/resources/queen_white.jpg");
    stone_black_select = new QPixmap(":/resources/stone_brown_select.jpg");
    stone_white_select = new QPixmap(":/resources/stone_white_select.jpg");
    queen_black_select = new QPixmap(":/resources/queen_brown_select.jpg");
    queen_white_select = new QPixmap(":/resources/queen_white_select.jpg");
    move_circle = new QPixmap(":/resources/move.jpg");
    jump_circle = new QPixmap(":/resources/jump.jpg");

    ui->tab1_g_layout->setSpacing(0);                   //  set zero spacing to grid layout which holds images

    if (high_resolution)                                //  high resolution
    {
        ui->tab1_g_layout->setMargin(5);                //  set concrete margin to grid layout which holds gaming board cells
        ui->tab2_g_layout->setMargin(0);                //  set concrete margin to grid layout which holds menu buttons
        ui->tab2_g_layout->setSpacing(30);              //  set concrete spacing to grid layout which holds menu buttons
    }
    else                                                //  standard resolution
    {
        ui->tab1_g_layout->setMargin(4);                //  set concrete margin to grid layout which holds gaming board cells
        ui->tab2_g_layout->setMargin(5);                //  set concrete margin to grid layout which holds menu buttons
        ui->tab2_g_layout->setSpacing(10);              //  set concrete spacing to grid layout which holds menu buttons
    }

    ui->tab3_v_layout->setMargin(0);                    //  set concrete margin to layout which holds about image
    ui->tab4_v_layout->setMargin(0);                    //  set concrete margin to layout which holds help image
}

void richers::create_icons()
{
}

void richers::init_arrays()
{
    selected = false;                                   //  no cell is selected
    white_playing = true;                               //  first player is on turn at start of game
    only_jumping = 0;                                   //  insane to jump at start of game
    turn_started = false;                               //  multiple jump is not possible at start of game

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
    switch_to_game();                                   //  move to game tab in tab widget
}

void richers::init_icons()
{
    for (int i = 0; i < 8; i++)                         //  iterate through all cells of gaming board
    {
        for (int j = 0; j < 8; j++)
        {
            if (high_resolution)                        //  high resolution
            {
                block[i][j].setMinimumSize(115, 115);   //  set fixed size of every cell
                block[i][j].setMaximumSize(115, 115);
            }
            else                                        //  standard resolution
            {
                block[i][j].setMinimumSize(84, 84);     //  set fixed size of every cell
                block[i][j].setMaximumSize(84, 84);
            }

            block[i][j].setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);  //  set alignment of image content
            block[i][j].setMargin(0);                   //  set zero margin for image
            block[i][j].setScaledContents(true);        //  scale image size

            if (whites[i][j])                           //  white stone have to be on this cell
                block[i][j].setPixmap(* stone_white);   //  insert white stone image
            else if (blacks[i][j])                      //  black stone have to be on this cell
                block[i][j].setPixmap(* stone_black);   //  insert black stone image
            else                                        //  this cell have to be empty
            {
                if (i & 1)                              //  odd line index
                {
                    if (j & 1)                          //  odd field index
                        block[i][j].setPixmap(* block_white);   //  insert empty white block
                    else                                //  even field index
                        block[i][j].setPixmap(* block_brown);   //  insert empty brown block
                }
                else                                    //  even line index
                {
                    if (j & 1)                          //  odd field index
                        block[i][j].setPixmap(* block_brown);   //  insert empty brown block
                    else                                //  even field index
                        block[i][j].setPixmap(* block_white);   //  insert empty white block
                }
            }

            block[i][j].line = i;                       //  load line index into image object attribute
            block[i][j].field = j;                      //  load field index into image object attribute
            ui->tab1_g_layout->addWidget(&(block[i][j]), i, j);     //  add image into grid layout

            QObject::connect(&(block[i][j]), SIGNAL(clicked(int,int)), this, SLOT(click_reaction(int, int)));   //  handle image click
        }
    }
}

void richers::init_menu()
{
    new_game_image = new QPixmap(":/resources/new.jpg");    //  allocate memory for menu buttons and set path to source images
    help_image = new QPixmap(":/resources/help.jpg");
    info_image = new QPixmap(":/resources/info.jpg");
    exit_image = new QPixmap(":/resources/exit.jpg");

    for (int i = 0; i < 4; i++)                         //  same behaviour for all menu buttons
    {
        menu_button[i].setMinimumSize(330, 330);        //  set fixed size of button
        menu_button[i].setMaximumSize(330, 330);
        menu_button[i].setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);  //  set alignment of button content
        menu_button[i].setMargin(0);                    //  set zero margin for button

        menu_button[i].line = 42 + i;                   //  exception for game logic -> handle menu button click

        QObject::connect(menu_button + i, SIGNAL(clicked(int,int)), this, SLOT(click_reaction(int, int)));   //  handle button click
    }

    menu_button[0].setPixmap(* new_game_image);         //  set image for new game button
    menu_button[1].setPixmap(* help_image);             //  set image for help button
    menu_button[2].setPixmap(* info_image);             //  set image for info button
    menu_button[3].setPixmap(* exit_image);             //  set image for exit button

    ui->tab2_g_layout->addWidget(menu_button, 0, 0);    //  add menu buttons into grid layout
    ui->tab2_g_layout->addWidget(menu_button + 1, 0, 1);
    ui->tab2_g_layout->addWidget(menu_button + 2, 1, 0);
    ui->tab2_g_layout->addWidget(menu_button + 3, 1, 1);
}

void richers::reset_board_color(int & x, int & y)
{
    if (whites[y][x])                                   //  white stone have to be on this cell
    {
        if (queens[y][x])                               //  queen
            block[y][x].setPixmap(* queen_white);       //  load white queen image on this cell
        else                                            //  stone
            block[y][x].setPixmap(* stone_white);       //  load white stone image on this cell
    }
    else if (blacks[y][x])                              //  black stone have to be on this cell
    {
        if (queens[y][x])                               //  queen
            block[y][x].setPixmap(* queen_black);       //  load black queen image on this cell
        else                                            //  stone
            block[y][x].setPixmap(* stone_black);       //  load black stone image on this cell
    }
    else                                                //  this cell have to be empty
        block[y][x].setPixmap(* block_brown);           //  insert empty brown block
}

void richers::switch_to_game()
{
    workspace = game;
    ui->tabs->setCurrentIndex(game);                    //  move to game tab in tab widget
    setPalette(black_palette);                          //  set background color to black
}

void richers::switch_to_menu()
{
    workspace = menu;
    ui->tabs->setCurrentIndex(menu);                    //  move to menu tab in tab widget
    setPalette(white_palette);                          //  set background color to white
}

void richers::jump_predicate()
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

bool richers::is_game_over()
{
    if (only_jumping == 0)                              //  jump predicate falsed
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

void richers::click_reaction(int y, int x)
{
    if (y == 42)                                        //  exception -> new game button pressed
    {
        new_game_button_click();                        //  execute new game routine
        return;                                         //  do not touch game logic
    }
    else if (y == 43)                                   //  exception -> help button pressed
    {
        help_button_click();                            //  execute help routine
        return;                                         //  do not touch game logic
    }
    else if (y == 44)                                   //  exception -> info button pressed
    {
        about_button_click();                           //  execute info routine
        return;                                         //  do not touch game logic
    }
    else if (y == 45)                                   //  exception -> exit button pressed
    {
        exit_button_click();                            //  execute exit routine
        return;                                         //  do not touch game logic
    }

    if (white_playing)                                  //  white player is on turn
    {
        if (whites[y][x] && turn_started == false)      //  it was clicked on white stone and there is no running multiple jump
        {
            if (queens[y][x])                           //  there is white queen on selected cell
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
                        block[y][x].setPixmap(* queen_white_select);    //  set new cell with white stone as selected
                    else
                        block[y][x].setPixmap(* stone_white_select);    //  set new cell with white queen as selected

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
                                block[y][x].setPixmap(* jump_circle);

                                while (x && y)          //  draw all possible jump cells to red
                                {
                                    if (blacks[y - 1][x - 1] || whites[y - 1][x - 1])
                                        break;

                                    temp.x = --x;       //  draw possible movement cell to red
                                    temp.y = --y;
                                    move_hint.push_back(temp);
                                    block[y][x].setPixmap(* jump_circle);
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
                                block[y][x].setPixmap(* jump_circle);

                                while (x < 7 && y)      //  draw all possible jump cells to red
                                {
                                    if (blacks[y - 1][x + 1] || whites[y - 1][x + 1])
                                        break;

                                    temp.x = ++x;       //  draw possible movement cell to red
                                    temp.y = --y;
                                    move_hint.push_back(temp);
                                    block[y][x].setPixmap(* jump_circle);
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
                                block[y][x].setPixmap(* jump_circle);

                                while (x && y < 7)      //  draw all possible jump cells to red
                                {
                                    if (blacks[y + 1][x - 1] || whites[y + 1][x - 1])
                                        break;

                                    temp.x = --x;       //  draw possible movement cell to red
                                    temp.y = ++y;
                                    move_hint.push_back(temp);
                                    block[y][x].setPixmap(* jump_circle);
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
                                block[y][x].setPixmap(* jump_circle);

                                while (x < 7 && y < 7)  //  draw all possible jump cells to red
                                {
                                    if (blacks[y + 1][x + 1] || whites[y + 1][x + 1])
                                        break;

                                    temp.x = ++x;       //  draw possible movement cell to red
                                    temp.y = ++y;
                                    move_hint.push_back(temp);
                                    block[y][x].setPixmap(* jump_circle);
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
                            block[y][x].setPixmap(* move_circle);
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
                            block[y][x].setPixmap(* move_circle);
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
                            block[y][x].setPixmap(* move_circle);
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
                            block[y][x].setPixmap(* move_circle);
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
                        block[y][x].setPixmap(* queen_white_select);    //  set new cell with white stone as selected
                    else
                        block[y][x].setPixmap(* stone_white_select);    //  set new cell with white queen as selected

                    if (only_jumping)                   //  jumping is requied when it is possible to jump
                    {
                        if (x > 1 && y < 6)             //  jumping left
                        {
                            if (blacks[y + 1][x - 1] && blacks[y + 2][x - 2] == false && whites[y + 2][x - 2] == false)
                            {
                                coordinates temp;       //  draw left move hint cell to red
                                temp.x = x - 2;
                                temp.y = y + 2;
                                move_hint.push_back(temp);

                                block[temp.y][temp.x].setPixmap(* jump_circle);
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

                                block[temp.y][temp.x].setPixmap(* jump_circle);
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
                                block[temp.y][temp.x].setPixmap(* move_circle);
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
                                block[temp.y][temp.x].setPixmap(* move_circle);
                            }
                        }
                    }
                }
            }
        }
        else if (selected)                              //  click on non-white stone cell when selected
        {
            int len = move_hint.size();

            for (int i = 0; i < len; i++)
            {                                           //  cell is in movement hints
                if (move_hint[i].x == x && move_hint[i].y == y)
                {
                    turn_started = true;                //  selecting another stone is disabled until actual turn will not end

                    block[select.y][select.x].setPixmap(* block_brown);

                    if (queens[select.y][select.x])
                    {
                        block[y][x].setPixmap(* queen_white);   //  refresh stones positions
                        queens[select.y][select.x] = false;
                        queens[y][x] = true;
                    }
                    else
                    {
                        if (y == 7)                     //  transform soldier to queen
                        {
                            queens[y][x] = true;
                            block[y][x].setPixmap(* queen_white);   //  set queen icon
                        }
                        else                            //  still soldier
                            block[y][x].setPixmap(* stone_white);   //  refresh stones positions
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

                                break;                  //  if stone is queen, then it will be removed after
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

                        int len = move_hint.size();

                        if (len)                        //  multiple jump is possible
                        {
                            select.x = x;               //  save coordinates
                            select.y = y;

                            if (queens[y][x])
                                block[y][x].setPixmap(* queen_white_select);    //  set new cell with white stone as selected
                            else
                                block[y][x].setPixmap(* stone_white_select);    //  set new cell with white queen as selected

                            for (int i = 0; i < len; i++)   //  draw all red move hint cells
                                block[move_hint[i].y][move_hint[i].x].setPixmap(* jump_circle);

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

                        int len = move_hint.size();

                        if (len)                        //  multiple jump is possible
                        {
                            select.x = x;               //  save coordinates
                            select.y = y;

                            if (queens[y][x])
                                block[y][x].setPixmap(* queen_white_select);    //  set new cell with white stone as selected
                            else
                                block[y][x].setPixmap(* stone_white_select);    //  set new cell with white queen as selected

                            for (int i = 0; i < len; i++)   //  draw all red move hint cells
                                block[move_hint[i].y][move_hint[i].x].setPixmap(* jump_circle);

                            return;                     //  white player is still playing
                        }
                    }

                    while (stones_to_remove.size())     //  remove all over jumped opponents stones
                    {
                        int temp_x = stones_to_remove.front().x;    //  save temporary coordinates
                        int temp_y = stones_to_remove.front().y;

                        block[temp_y][temp_x].setPixmap(* block_brown);
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
                        block[y][x].setPixmap(* queen_black_select);    //  set new cell with black stone as selected
                    else
                        block[y][x].setPixmap(* stone_black_select);    //  set new cell with black queen as selected

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
                                block[y][x].setPixmap(* jump_circle);

                                while (x && y)          //  draw all possible jump cells to red
                                {
                                    if (whites[y - 1][x - 1] || blacks[y - 1][x - 1])
                                        break;

                                    temp.x = --x;       //  draw possible movement cell to red
                                    temp.y = --y;
                                    move_hint.push_back(temp);
                                    block[y][x].setPixmap(* jump_circle);
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
                                block[y][x].setPixmap(* jump_circle);

                                while (x < 7 && y)      //  draw all possible jump cells to red
                                {
                                    if (whites[y - 1][x + 1] || blacks[y - 1][x + 1])
                                        break;

                                    temp.x = ++x;       //  draw possible movement cell to red
                                    temp.y = --y;
                                    move_hint.push_back(temp);
                                    block[y][x].setPixmap(* jump_circle);
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
                                block[y][x].setPixmap(* jump_circle);

                                while (x && y < 7)      //  draw all possible jump cells to red
                                {
                                    if (whites[y + 1][x - 1] || blacks[y + 1][x - 1])
                                        break;

                                    temp.x = --x;       //  draw possible movement cell to red
                                    temp.y = ++y;
                                    move_hint.push_back(temp);
                                    block[y][x].setPixmap(* jump_circle);
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
                                block[y][x].setPixmap(* jump_circle);

                                while (x < 7 && y < 7)  //  draw all possible jump cells to red
                                {
                                    if (whites[y + 1][x + 1] || blacks[y + 1][x + 1])
                                        break;

                                    temp.x = ++x;       //  draw possible movement cell to red
                                    temp.y = ++y;
                                    move_hint.push_back(temp);
                                    block[y][x].setPixmap(* jump_circle);
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
                            block[y][x].setPixmap(* move_circle);
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
                            block[y][x].setPixmap(* move_circle);
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
                            block[y][x].setPixmap(* move_circle);
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
                            block[y][x].setPixmap(* move_circle);
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
                        block[y][x].setPixmap(* queen_black_select);    //  set new cell with black stone as selected
                    else
                        block[y][x].setPixmap(* stone_black_select);    //  set new cell with black queen as selected

                    if (only_jumping)                   //  jumping is requied when it is possible to jump
                    {
                        if (x > 1 && y > 1)             //  jumping left
                        {
                            if (whites[y - 1][x - 1] && whites[y - 2][x - 2] == false && blacks[y - 2][x - 2] == false)
                            {
                                coordinates temp;       //  draw left move hint cell to red
                                temp.x = x - 2;
                                temp.y = y - 2;
                                move_hint.push_back(temp);

                                block[temp.y][temp.x].setPixmap(* jump_circle);
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

                                block[temp.y][temp.x].setPixmap(* jump_circle);
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
                                block[temp.y][temp.x].setPixmap(* move_circle);
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
                                block[temp.y][temp.x].setPixmap(* move_circle);
                            }
                        }
                    }
                }
            }
        }
        else if (selected)                              //  click on non-black stone cell when selected
        {
            int len = move_hint.size();

            for (int i = 0; i < len; i++)
            {                                           //  cell is in movement hints
                if (move_hint[i].x == x && move_hint[i].y == y)
                {
                    turn_started = true;                //  selecting another stone is disabled until actual turn will not end

                    block[select.y][select.x].setPixmap(* block_brown);

                    if (queens[select.y][select.x])
                    {
                        block[y][x].setPixmap(* queen_black);   //  refresh stones positions
                        queens[select.y][select.x] = false;
                        queens[y][x] = true;
                    }
                    else
                    {
                        if (y == 0)                     //  transform soldier to queen
                        {
                            queens[y][x] = true;
                            block[y][x].setPixmap(* queen_black);   //  set queen icon
                        }
                        else                            //  still soldier
                            block[y][x].setPixmap(* stone_black);   //  refresh stones positions
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

                                break;                  //  if stone is queen, then it will be removed after
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

                        int len = move_hint.size();

                        if (len)                        //  multiple jump is possible
                        {
                            select.x = x;               //  save coordinates
                            select.y = y;

                            if (queens[y][x])
                                block[y][x].setPixmap(* queen_black_select);    //  set new cell with black stone as selected
                            else
                                block[y][x].setPixmap(* stone_black_select);    //  set new cell with black queen as selected

                            for (int i = 0; i < len; i++)   //  draw all red move hint cells
                                block[move_hint[i].y][move_hint[i].x].setPixmap(* jump_circle);

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

                        int len = move_hint.size();

                        if (len)                        //  multiple jump is possible
                        {
                            select.x = x;               //  save coordinates
                            select.y = y;

                            if (queens[y][x])
                                block[y][x].setPixmap(* queen_black_select);    //  set new cell with black stone as selected
                            else
                                block[y][x].setPixmap(* stone_black_select);    //  set new cell with black queen as selected

                            for (int i = 0; i < len; i++)   //  draw all red move hint cells
                                block[move_hint[i].y][move_hint[i].x].setPixmap(* jump_circle);

                            return;                     //  black player is still playing
                        }
                    }

                    while (stones_to_remove.size())     //  remove all over jumped opponents stones
                    {
                        int temp_x = stones_to_remove.front().x;    //  save temporary coordinates
                        int temp_y = stones_to_remove.front().y;

                        block[temp_y][temp_x].setPixmap(* block_brown);
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

void richers::new_game_button_click()
{
    init_arrays();                                      //  set default positions of stones
    init_icons();

    switch_to_game();                                   //  move to game tab in tab widget
}

void richers::help_button_click()
{
    workspace = help;
    ui->tabs->setCurrentIndex(help);                    //  move to help tab in tab widget
}

void richers::about_button_click()
{
    workspace = about;
    ui->tabs->setCurrentIndex(about);                   //  move to about tab in tab widget
    setWindowTitle("Version 1.8");
}

void richers::exit_button_click()
{
    QApplication::quit();                               //  immediately quit application
}
