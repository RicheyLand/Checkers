#include "richers.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    richers * w = new richers;
    w->show();

    return a.exec();
}
