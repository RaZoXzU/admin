#include "mainwindow.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    // Don't touch this file !!!
    // --- Dodanie obsługi formularzy --- //

    QApplication a(argc, argv);
    a.addLibraryPath(QCoreApplication::applicationDirPath() +"/plugins/");

    MainWindow w;
    w.show();

    return a.exec();
  // --- Pierwszy główny -------------- //
}
