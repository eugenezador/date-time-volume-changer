#ifndef VOLUME_CHANGER_H
#define VOLUME_CHANGER_H

#include <QObject>
#include <QProcess>
#include <QDebug>
#include <fstream>

class Volume_changer : public QObject
{
    Q_OBJECT
public:
    explicit Volume_changer(QObject *parent = nullptr);
    ~Volume_changer();

    void get_cur_sound_value();

    void set_sink_mute();

    void set_volume(char znak);

    int is_number(QCharRef c);

signals:
    void upd_sound_slider(int sound_value);

public slots:
    void raise();


public:
    bool is_tunning_volume = false;

private:
    QProcess *p = nullptr;

    QStringList args;

    QString new_sound_value;

    int current_sound_val;

};

#endif // VOLUME_CHANGER_H
