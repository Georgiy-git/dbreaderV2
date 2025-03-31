#pragma once

#include <QDialog>
#include <QVBoxLayout>
#include <QList>
#include <QLineEdit>
#include <QString>
#include <QComboBox>

namespace Ui {
class CreateTable;
}

class CreateTable : public QDialog
{
    Q_OBJECT

public:
    CreateTable();

    ~CreateTable();

private slots:
    void on_spinBox_valueChanged(int arg1);

    void on_buttonBox_accepted();

private:
    Ui::CreateTable *ui;
    QWidget* v;
    QVBoxLayout* layout;
    QList<QLineEdit*> list;
    QList<QComboBox*> list_combo;
    int y {};
    QList<QChar> invalid = {',', '(', ')', ';', ':'};

signals:
    void signal_open_db(QString);
};
