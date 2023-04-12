#include "dialogsettingtime.h"
#include "ui_dialogsettingtime.h"
#include <QProcess>
#include <QThread>

DialogSettingTime::DialogSettingTime(QWidget *parent) :
    QDialog(parent),
    tmr(),
    ui(new Ui::DialogSettingTime) ,
    volume_changer(new Volume_changer(this))
{
    ui->setupUi(this);
    //    connect(ui->pb_Rabota,SIGNAL(clicked(bool)),ui->dateTimeEdit,SLOT(moveUpOrSetTime()));
    //    connect(ui->pb_Sbros,SIGNAL(clicked(bool)),ui->dateTimeEdit,SLOT(nextSection()));
    //    ui->dateTimeEdit->sectionText()
    connect(ui->dateTimeEdit,SIGNAL(timeSetted()),
            this,SLOT(hide()));
    ui->dateTimeEdit->setLabel(ui->label);
//    QTimer::singleShot(10,this,SLOT(hide()));  //TODO :: 5ka need this line
    tmr.setSingleShot(true);
    //    tmr.s
connect(this, SIGNAL(change_volume()), volume_changer, SLOT(raise()));
//    connect(this, &DialogSettingTime::change_volume, volume_changer, &Volume_changer::raise);

}

DialogSettingTime::~DialogSettingTime()
{
    delete ui;
}

void DialogSettingTime::nextSection()
{
    qDebug() << __FUNCTION__;
    static int numOfClicks = 0;
    if (!this->isHidden()) {

        if(dateTimeColumsCounter == 6) {
            emit change_volume();



        } else {
            ui->dateTimeEdit->nextSection();
            dateTimeColumsCounter++;
        }

    }
    else
    {
        if(!tmr.isActive())
        {
            tmr.setInterval(2000);
            tmr.start();
            numOfClicks = 0;
        }
        else
        {
            numOfClicks++;
            qDebug() << "numOfClicks = " << numOfClicks;
            if (numOfClicks>4) this->show();
        }
    }
}

void DialogSettingTime::moveUpOrSetTime()
{
    if (!this->isHidden()) {
        ui->dateTimeEdit->moveUpOrSetTime();

    }
}

DateTimeSettingWidget::DateTimeSettingWidget(QWidget *parent) : QDateTimeEdit(parent),
    waitingProve(false)
{
    this->setDisplayFormat("yyyy.MM.dd HH:mm:ss");
    this->setDateTime(QDateTime::fromString(QString("2020.06.01 09:00:00"),QString("yyyy.MM.dd HH:mm:ss")));

}

void DateTimeSettingWidget::setLabel(QLabel *in)
{
    lbl = in;
    lbl->setWordWrap(true);
    lbl->setText(QString("НАЖМИ РАБОТА ДЛЯ ПРИБАВЛЕНИЯ ГОДА СБРОС ДЛЯ ПЕРЕХОДА К МЕСЯЦУ"));
}

void DateTimeSettingWidget::nextSection()
{
    Section sec = this->currentSection();
    if (sec == QDateTimeEdit::YearSection)
    {
        lbl->setText(QString("НАЖМИ РАБОТА ДЛЯ ПРИБАВЛЕНИЯ МЕСЯЦА СБРОС ДЛЯ ПЕРЕХОДА К ЧИСЛУ"));
        this->setSelectedSection(QDateTimeEdit::MonthSection);
    }
    else if (sec == QDateTimeEdit::MonthSection)
    {
        lbl->setText(QString("НАЖМИ РАБОТА ДЛЯ ПРИБАВЛЕНИЯ ЧИСЛА СБРОС ДЛЯ ПЕРЕХОДА К ЧАСУ"));
        this->setSelectedSection(QDateTimeEdit::DaySection);
    }
    else if (sec == QDateTimeEdit::DaySection)
    {
        lbl->setText(QString("НАЖМИ РАБОТА ДЛЯ ПРИБАВЛЕНИЯ ЧАСА СБРОС ДЛЯ ПЕРЕХОДА К МИНУТАМ"));
        this->setSelectedSection(QDateTimeEdit::HourSection);
    }
    else if (sec == QDateTimeEdit::HourSection)
    {
        lbl->setText(QString("НАЖМИ РАБОТА ДЛЯ ПРИБАВЛЕНИЯ МИНУТЫ СБРОС ДЛЯ ПЕРЕХОДА К СЕКУНДАМ"));
        this->setSelectedSection(QDateTimeEdit::MinuteSection);
    }
    else if (sec == QDateTimeEdit::MinuteSection)
    {
        lbl->setText(QString("НАЖМИ РАБОТА ДЛЯ ПРИБАВЛЕНИЯ СЕКУНДЫ СБРОС ДЛЯ ПЕРЕХОДУ К ВЫБОРУ УСТАНОВЛЕНИЯ ВРЕМЕНИ"));
        this->setSelectedSection(QDateTimeEdit::SecondSection);
    }
    else if (sec == QDateTimeEdit::SecondSection && !waitingProve)
    {
        lbl->setText(QString("НАЖМИ СТАРТ ДЛЯ ПОДТВЕРЖДЕНИЯ ИЛИ СБРОС ДЛЯ УСТАНОВКИ ГОДА"));
        lbl->setFocus();
        waitingProve = true;

//        emit change_volume();
    }
    else if (waitingProve)
    {
        this->setSelectedSection(QDateTimeEdit::YearSection);
        this->setDateTime(QDateTime::fromString(QString("2020.06.01 09:00:00"),QString("yyyy.MM.dd HH:mm:ss")));
        waitingProve = false;
        lbl->setText(QString("НАЖМИ РАБОТА ДЛЯ ПРИБАВЛЕНИЯ ГОДА СБРОС ДЛЯ ПЕРЕХОДА К МЕСЯЦУ"));
    }


}

void DateTimeSettingWidget::moveUpOrSetTime()
{
    QDateTime dataState=this->dateTime();
    if (waitingProve)
    {
        FILE *fileToSetTime = nullptr;
        fileToSetTime = fopen("set_date_time.sh","wt");
        if (fileToSetTime != nullptr)
        {
            fprintf(fileToSetTime,"#!/bin/sh\n\n");
            fprintf(fileToSetTime,"date +%%Y%%m%%d -s \"%4d%02d%02d\"\n",dataState.date().year(),dataState.date().month(),dataState.date().day());
            fprintf(fileToSetTime,"date +%%T -s \"%02d:%02d:%02d\"\n",dataState.time().hour(),dataState.time().minute(),dataState.time().second());
            fclose(fileToSetTime);
            QProcess::execute("sudo ./set_date_time.sh");
            thread()->sleep(1);
            QProcess::execute("sudo hwclock --systohc");
        }
        emit timeSetted();
    }
    else
    {
        stepUp();
    }
}
