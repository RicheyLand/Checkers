#include "checkers.h"

int main(int argc, char ** argv)
{
    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");

    Checkers checkers;

    return app->run(checkers);
}
