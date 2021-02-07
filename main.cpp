#include "login_win.h"

#include <QApplication>
#include "log.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Login_win w;
    w.show();
    return a.exec();
}
