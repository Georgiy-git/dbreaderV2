#include "opendb.hpp"
#include "table.hpp"
#include "ui_table.h"

#include <QmessageBox>
#include <QtSql>
#include <QSqlTableModel>

Table::Table() : ui(new Ui::Table)
{
    ui->setupUi(this);
    ui->verticalLayout->setAlignment(Qt::AlignTop);
    ui->label->setText(" БД не открыта ");
    sqlite3_open("history.db", &history_db);
    sqlite3_exec(history_db, "CREATE TABLE IF NOT EXISTS info1 (path TEXT)", nullptr, nullptr, nullptr);
    sqlite3_exec(history_db, "SELECT * FROM info1", _get_data, this, nullptr);
}

Table::~Table()
{
    delete ui;
}

void Table::open_db(QString db_name)
{
    db.setDatabaseName(db_name);

    if (!db.open())
    {
        QMessageBox::critical(nullptr, "Ошибка", "Ошибка подключения к Базе Данных: " + db.lastError().text());
        return;
    }
    tables.clear();
    _db_name = db_name;

    this_table = db_name.mid(db_name.lastIndexOf('/')+1);

    ui->label->setText(this_table);

    QSqlQuery query("SELECT name FROM sqlite_master WHERE type='table'");
    while (query.next())
    {
        QString table_name = query.value(0).toString();
        std::shared_ptr<QPushButton> table_button = std::make_shared<QPushButton>(table_name);
        connect(table_button.get(), &QPushButton::clicked, this,
                [this, table_name]{_set_table_model(table_name);});
        table_button->setStyleSheet("QPushButton {"
                                    "background-color: green;"
                                    "border: none;"
                                    "color: white;"
                                    "margin: 1px 1px;"
                                    "border-radius: 5px;"
                                    "        }"
                                    "QPushButton:hover {"
                                    "background-color: lightgreen;"
                                    "        }"
                                    "QPushButton:pressed {"
                                    "background-color: darkgreen;"
                                            "}");
        tables.push_back(table_button);
        ui->verticalLayout->addWidget(table_button.get());
        sqlite3_exec(history_db, "DELETE FROM info1", nullptr, nullptr, nullptr);
        std::string _sql = "INSERT INTO info1 (path) VALUES ('";
        _sql += _db_name.toStdString();
        _sql += "')";
        sqlite3_exec(history_db, _sql.c_str(), nullptr, nullptr, nullptr);
    }
}

//Открыть БД
void Table::on_action_triggered()
{
    dialog1 = new OpenDb;
    connect(dialog1, &OpenDb::signal_open_db, this, open_db);
    dialog1->show();
}

int Table::_get_data(void *obj, int count, char **data, char **column_name)
{
    Table* x = static_cast<Table*>(obj);
    x->last_table = QString::fromUtf8(data[0]);
    return 0;
}

void Table::_set_table_model(QString _table_name)
{
    if (sql_table_model != nullptr) { delete sql_table_model; }
    sql_table_model = new QSqlTableModel(this, db);
    sql_table_model->setTable(_table_name);
    sql_table_model->select();
    ui->tableView->setModel(sql_table_model);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}


void Table::on_action_14_triggered()
{
    sqlite3_exec(history_db, "SELECT * FROM info1", _get_data, this, nullptr);
    if (!last_table.isEmpty()) open_db(last_table);
}

