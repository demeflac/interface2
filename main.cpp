#include "interface2.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName("github.com/demeflac");
    app.setApplicationName("interface2");
    Interface2 w;
    w.show();
    return app.exec();
}




