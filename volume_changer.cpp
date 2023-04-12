#include "volume_changer.h"

Volume_changer::Volume_changer(QObject *parent)
    : QObject{parent}
{

}

Volume_changer::~Volume_changer()
{
    delete p;
}

void Volume_changer::get_cur_sound_value()
{


    args.clear();
    args << "get-sink-volume" << "@DEFAULT_SINK@"
//         << "|" << "awk '{print $5}'"
//         << ">>" << "cur_sound_value.txt"
            ;

    p->start("pactl", args, QIODevice::ReadOnly);
    p->waitForFinished();


    QString StdOut      =   p->readAllStandardOutput();  //Reads standard output
    QString StdError    =   p->readAllStandardError();   //Reads standard error

    qDebug() << "\n Printing the standard output..........\n";
    qDebug() << "\n" << StdOut;
    qDebug() << "\n Printing the standard error..........\n";
    qDebug() << "\n" << StdError;

    QString temp = {'\0'};
    int k = 0;
            if(is_number(StdOut[28])){
                temp[k++] = StdOut[28];
            }

            if(is_number(StdOut[29])) {
                temp[k++] = StdOut[29];
            }

            if(is_number(StdOut[30])) {
            temp[k++] = StdOut[30];
            }

    qDebug() << "temp str = " << temp;

    current_sound_val = temp.toInt();
    qDebug() << "vol val = " << current_sound_val;

}

void Volume_changer::set_sink_mute()
{
    args.clear();
    args << "set-sink-mute" << "@DEFAULT_SINK@" << "0";
    p->start("pactl",args,QIODevice::ReadOnly);
    p->waitForFinished();
}

void Volume_changer::set_volume(char znak)
{
    if(znak == '+') {
        ++current_sound_val;
    } else {
        --current_sound_val;
    }

    new_sound_value.clear();
    new_sound_value = QString::number(current_sound_val);
    new_sound_value.append("%");

    args.clear();
    args << "set-sink-volume" << "@DEFAULT_SINK@" << new_sound_value;
    p->start("pactl",args,QIODevice::ReadOnly);
    p->waitForFinished();
}

int Volume_changer::is_number(QCharRef c)
{
    int result = 0;
    if(c >= '0' && c <= '9') {
        result = 1;
    }
    return result;
}

void Volume_changer::raise()
{
    set_sink_mute();

    get_cur_sound_value();

    set_volume('+');
}
