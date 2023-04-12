#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    dst = new DialogSettingTime(this);
    dst->show();
    connect(ui->pb_Sbros,SIGNAL(clicked(bool)),dst,SLOT(nextSection()));
    connect(ui->pb_Rabota,SIGNAL(clicked(bool)),dst,SLOT(moveUpOrSetTime()));
}

Dialog::~Dialog()
{
    delete ui;
}
