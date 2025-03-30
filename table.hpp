#pragma once

#include <QMainWindow>
#include <QPushButton>

class OpenDb;

namespace Ui {
class Table;
}

class Table : public QMainWindow
{
    Q_OBJECT

public:
    Table();
    ~Table();

    void open_db(QString db_name);

private slots:
    void on_action_triggered();

private:
    Ui::Table *ui;
    OpenDb* dialog1;
    std::vector<std::shared_ptr<QPushButton>> tables;
    QString this_table;
};
