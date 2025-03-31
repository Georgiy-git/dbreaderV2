#pragma once

#include <QMainWindow>
#include <QPushButton>
#include <QSqlTableModel>
#include "sqlite3.h"

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
    void on_action_triggered(); //Открыть БД

    void on_action_14_triggered();

private:
    Ui::Table *ui;
    OpenDb* dialog1;
    std::vector<std::shared_ptr<QPushButton>> tables;
    QString this_table;
    QSqlTableModel* sql_table_model = nullptr;
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    sqlite3* history_db;
    QString last_table;
    QString _db_name;

    static int _get_data(void* obj, int count, char** data, char** column_name);

    void _set_table_model(QString _table_name);
};
