#include "opendb.hpp"
#include "ui_opendb.h"

#include <QFileDialog>
#include <QMessageBox>

OpenDb::OpenDb(sqlite3 *history_db) : ui(new Ui::OpenDb), history_db{history_db}
{
    ui->setupUi(this);
    setFixedSize(400, 150);
    ui->lineEdit->setPlaceholderText("Введите путь к БД ");
    disconnect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);

    sqlite3_exec(history_db, "CREATE TABLE IF NOT EXISTS info2 (path TEXT UNIQUE)", nullptr, nullptr, nullptr);
    sqlite3_exec(history_db, "SELECT * FROM info2", _get_data, this, nullptr);

    model = std::make_unique<QStringListModel>(history_tables);
    completer = std::make_unique<QCompleter>(model.get());
    completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
    ui->lineEdit->setCompleter(completer.get());
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

    std::string _sql = "REPLACE INTO info2 (path) VALUES ('";
    _sql += db_name.toStdString();
    _sql += "')";
    sqlite3_exec(history_db, _sql.c_str(), nullptr, nullptr, nullptr);

    emit signal_open_db(db_name);
    close();
}

int OpenDb::_get_data(void *obj, int count, char **data, char **column_name)
{
    OpenDb* x = static_cast<OpenDb*>(obj);
    x->history_tables.push_back(QString::fromUtf8(data[0]));
    return 0;
}

