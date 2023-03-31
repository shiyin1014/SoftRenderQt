#include <QApplication>
#include "viewclass.h"

using namespace std;

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    viewClass view;
    view.show();
    return QApplication::exec();
}
