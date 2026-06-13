#include "mainwindow.h"
#include <QApplication>

/**
 * @brief main function - launches the C-Cart application.
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
