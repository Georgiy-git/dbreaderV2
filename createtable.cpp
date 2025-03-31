#include "createtable.hpp"
#include "ui_createtable.h"

#include <QLabel>
#include <QMessageBox>
#include <iostream>
#include <QComboBox>
#include "sqlite3.h"

CreateTable::CreateTable() : ui(new Ui::CreateTable)
{
    ui->setupUi(this);

    disconnect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);

    v = ui->scrollArea->widget();

    layout = new QVBoxLayout(v);
    layout->setAlignment(Qt::AlignTop);
    v->setLayout(layout);
}

CreateTable::~CreateTable()
{
    delete ui;
    delete this;
}


void CreateTable::on_spinBox_valueChanged(int x)
{
    if (x < y)
    {
        for (int i = 1; i<4; i++)
        {
            QLayoutItem *item = layout->takeAt(y*3-i);
            delete item->widget();
            delete item;
        }
        list.pop_back();
        list_combo.pop_back();
    }

    if (x > y)
    {
        QLabel *l = new QLabel(QString::number(x)+"-й столбец:", this);
        QLineEdit *le = new QLineEdit(this);
        le->setStyleSheet("border: none;");
        QComboBox *c = new QComboBox(this);
        c->addItems({"TEXT", "NUMERIC", "INTEGER", "REAL", "BLOB"});
        list.append(le);
        list_combo.append(c);

        layout->addWidget(l);
        layout->addWidget(le);
        layout->addWidget(c);
        v->setLayout(layout);
    }

    y = x;
}

void CreateTable::on_buttonBox_accepted()
{
    bool ok = true;
    bool isempty = false;
    bool isinvalid = false;

    auto empty = [&ok]{QMessageBox::information(nullptr, "Ошибка", "Поля не могут быть пустыми ");
    ok = false;};

    auto inv = [&ok]{QMessageBox::information(nullptr, "Ошибка",
    "В названии столбца содержится недопустимый знак!");
    ok = false;};

    if (ui->lineEdit->text().trimmed().isEmpty()) isempty = true;

    for (auto i : invalid)
    {
        if (ui->lineEdit_2->text().contains(i)) isinvalid = true;
    }
    if (ui->lineEdit_2->text().trimmed().isEmpty()) isempty = true;


    QString column("CREATE TABLE IF NOT EXISTS "+ ui->lineEdit_2->text() +
                   " (ID INTEGER PRIMARY KEY AUTOINCREMENT");


    for (int i = 0; i<list.size(); i++) {
        for (auto j : invalid)
        {
            if (list[i]->text().contains(j)) {isinvalid = true; break;}

            if (list[i]->text().trimmed().isEmpty()) {isempty = true; break;}
        }
        column.append(", ")
              .append(list[i]->text())
              .append(" "+ list_combo[i]->currentText());
    }

    if (isempty) empty();
    if (isinvalid) inv();

    if (ok)
    {
    column.append(");");

    sqlite3* db;
    const char* _path = ui->lineEdit->text().trimmed().toUtf8();
    sqlite3_open(_path, &db);
    const char* _sql = column.toUtf8();
    sqlite3_exec(db, _sql, nullptr, nullptr, nullptr);
    sqlite3_close(db);

    emit signal_open_db(ui->lineEdit->text().trimmed());
    close();
    }
}

