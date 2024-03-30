#include "widget.h"
#include "ui_widget.h"

#include <QtMultimedia>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QAudioInput>
#include <QIODevice>
#include <QAudio>

#include "rtc/rtc.h"

#include "Peer.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    peer = new Peer(1);
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_clicked()
{
    peer->createDataChannel("mydc");
    peer->setLocalToOffer();

    string mySdp = peer->getSdp();
    ui->textBrowser->setText(QString::fromUtf8(mySdp.c_str()));
    peer->printLog();
}

void Widget::on_play_button_clicked()
{

}


void Widget::on_textEdit_textChanged()
{
}


void Widget::on_pushButton_2_clicked()
{
    peer->setRemoteDescription(ui->textEdit->toPlainText().toStdString());
}

void Widget::on_pushButton_3_clicked()
{
    peer->dc->onMessage([this](std::variant<rtc::binary, rtc::string> message) {
        if (std::holds_alternative<rtc::string>(message)) {
            string msgStr = get<rtc::string>(message);
            ui->textEdit->setText(QString::fromUtf8(msgStr.c_str()));
        }
    });

    peer->sendMsg(ui->textEdit_2->toPlainText().toStdString());
}
