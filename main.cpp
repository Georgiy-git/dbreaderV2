#include <QApplication>
#include "table.hpp"

int main(int args, char** argv)
{
    QApplication app(args, argv);
    Table table;
    table.show();
    app.exec();
}
