#include "widget.h"
#include "ui_widget.h"

#include <QtMultimedia>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QAudioInput>
#include <QIODevice>
#include <QAudio>

#include "rtc/rtc.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_play_button_clicked()
{
    // rtc::Configuration config;
    // config.iceServers.emplace_back("mystunserver.org:3478");

    // rtc::PeerConnection pc(config);
}


void Widget::on_pushButton_clicked()
{

}

