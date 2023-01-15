#include <QApplication>
#include "menu_window.hpp"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    MenuWindow window(8, 33, 300);
    return app.exec();
}
