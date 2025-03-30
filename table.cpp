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
}

Table::~Table()
{
    delete ui;
}

void Table::open_db(QString db_name)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(db_name);

    if (!db.open())
    {
        QMessageBox::critical(nullptr, "Ошибка", "Ошибка подключения к Базе Данных: " + db.lastError().text());
        return;
    }


    this_table = db_name.mid(db_name.lastIndexOf('/')+1);

    ui->label->setText(this_table);

    QSqlQuery query("SELECT name FROM sqlite_master WHERE type='table'");
    while (query.next()) {
        QString table_name = query.value(0).toString();
        std::shared_ptr<QPushButton> table_button = std::make_shared<QPushButton>(table_name);
        tables.push_back(table_button);
        ui->verticalLayout->addWidget(table_button.get());
        }

    // QSqlTableModel *model = new QSqlTableModel(this, db);
    // model->setTable(table_name);
    // model->select();
    // ui->tableView->setModel(model);
    // ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

//Открыть БД
void Table::on_action_triggered()
{
    dialog1 = new OpenDb;
    connect(dialog1, &OpenDb::signal_open_db, this, open_db);
    dialog1->show();
}

