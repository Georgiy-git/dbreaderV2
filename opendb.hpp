#ifndef OPENDB_HPP
#define OPENDB_HPP

#include <QDialog>
#include "sqlite3.h"
#include <QStringListModel>
#include <QCompleter>

namespace Ui {
class OpenDb;
}

class OpenDb : public QDialog
{
    Q_OBJECT

public:
    OpenDb(sqlite3 *history_db);
    ~OpenDb();

private slots:
    void on_pushButton_clicked();

    void on_buttonBox_accepted();

private:
    Ui::OpenDb *ui;
    sqlite3* history_db;
    std::unique_ptr<QStringListModel> model;
    QStringList history_tables;
    std::unique_ptr<QCompleter> completer;

    static int _get_data(void* obj, int count, char** data, char** column_name);

signals:
    void signal_open_db(QString);
};

#endif // OPENDB_HPP
