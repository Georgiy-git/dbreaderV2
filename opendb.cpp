#include "opendb.hpp"
#include "ui_opendb.h"

#include <QFileDialog>
#include <QMessageBox>

OpenDb::OpenDb() : ui(new Ui::OpenDb)
{
    ui->setupUi(this);
    setFixedSize(400, 150);
    ui->lineEdit->setPlaceholderText("Введите путь к БД ");
    disconnect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
}

OpenDb::~OpenDb()
{
    delete ui;
    delete this;
}

void OpenDb::on_pushButton_clicked()
{
    QString s = QFileDialog::getOpenFileName(
        this,
        "Открыть файл",
        QDir::homePath(),
        "Базы данных (*.db *.sqlite);;Все файлы (*)"
    );
   ui->lineEdit->setText(s);
}


void OpenDb::on_buttonBox_accepted()
{
    QString db_name = ui->lineEdit->text();

    if (db_name.trimmed().isEmpty()) {
        QMessageBox::information(this, "Ошибка", "Поле не может быть пустым ");
        return;
    } else if (!QFile::exists(db_name)) {
        QMessageBox::information(this, "Ошибка", "Файл не найден ");
        return;
    }

    emit signal_open_db(db_name);
    close();
}

