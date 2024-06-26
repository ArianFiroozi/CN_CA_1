#include "widget.h"
#include "ui_widget.h"

#include <QtMultimedia>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QAudioInput>
#include <QIODevice>
#include <QAudio>
#include <QPalette>
#include <QLineEdit>
#include <QMediaDevices>

#include "rtc/rtc.h"

#include "Peer.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    peer = new Peer();
    buffer = new QBuffer();

    QAudioFormat format;
    format.setSampleRate(8000);
    format.setChannelCount(1);
    format.setSampleFormat(QAudioFormat::UInt8);
    source = new QAudioSource(format, this);
    cout<<"buffer opened:"<<buffer->open(QIODevice::ReadWrite)<<endl;
    cout<<"buffer state: "<<buffer->isOpen()<<endl;
    source->start(buffer);

    ui->setupUi(this);
    connect(peer, &Peer::variableChanged, this->ui->message_display, &QTextBrowser::setText);
    connect(peer, &Peer::voiceReceived, this, &Widget::playAudio);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::create_first_track()
{
    const rtc::SSRC ssrc = 42;
    rtc::Description::Audio media("audio", rtc::Description::Direction::SendRecv);
    media.addSSRC(ssrc, "audio-send");

    peer->send_track = peer->pc->addTrack(media);
    peer->send_track->onOpen([]() {
        cout<<"send track opened"<<endl;
    });

    peer->send_track->onMessage([](std::variant<rtc::binary, rtc::string> message) {
        cout<<"received from send!"<<endl;
    });

    rtc::Description::Audio rec_media("audio", rtc::Description::Direction::SendRecv);
    rec_media.addSSRC(ssrc, "audio-rec");

    peer->receive_track = peer->pc->addTrack(rec_media);
    peer->receive_track->onOpen([]() {
        cout<<"rec track opened"<<endl;
    });

    peer->receive_track->onMessage(
        [this](rtc::binary message) {
            cout<<"binary message recieved"<<endl;
            playAudio(message);
        },
        [](rtc::string msg){
            cout<<"msgstr"<<msg<<endl;
        });
}

void Widget::create_second_track()
{
    const rtc::SSRC ssrc = 42;
    rtc::Description::Audio rec_media("audio", rtc::Description::Direction::SendRecv);
    rec_media.addSSRC(ssrc, "audio-receive");

    peer->send_track = peer->pc->addTrack(rec_media);
    auto session = std::make_shared<rtc::RtcpReceivingSession>();
    peer->send_track->setMediaHandler(session);
}

void Widget::on_show_sdp_clicked()
{

    if (ui->connect_peer->isEnabled())
        create_first_track();

    peer->createDataChannel("mydc");
    peer->setLocalToOffer();

    if (!ui->connect_peer->isEnabled())
        create_second_track();

    string mySdp = peer->getSdp();
    ui->textBrowser->setText(QString::fromUtf8(mySdp.c_str()));
    peer->printLog();
}

void Widget::on_connect_peer_clicked()
{
    peer->setRemoteDescription(ui->textEdit->toPlainText().toStdString());
    ui->connect_peer->setDisabled(true);
    ui->connect_peer->setText("Connected");
}

void Widget::on_send_text_clicked()
{
    peer->sendMsg(ui->textEdit_2->toPlainText().toStdString());
}

void Widget::on_send_audio_clicked()
{
    // while(1){
    //     std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    cout<<"buffer opened:"<<buffer->open(QIODevice::ReadWrite)<<endl;
    buffer->seek(0);
    QByteArray audioData;// = buffer->readAll();
    audioData.setRawData(buffer->readAll().toStdString().c_str(), 1000);
    if (!audioData.isEmpty()) {
        cout<<"sending audio!"<<audioData.size()<<endl;
        std::string rtpPacket(audioData.begin(), audioData.begin() + 1000);

        if (peer->send_track->isOpen())
            cout<<"send is:"<<peer->send_track->send(std::move(audioData.data()))<<endl;
        else
            cout<<"track not open"<<endl;

        buffer->buffer().clear();
    }
    // }
}

void Widget::playAudio(rtc::binary voiceBin)
{
    cout<<"inside playAudio"<<endl;

    QAudioFormat format;
    format.setSampleRate(8000);
    format.setChannelCount(1);
    format.setSampleFormat(QAudioFormat::UInt8);

    cout<<"message size: " << voiceBin.size()<<endl;
    QByteArray voiceArray = QByteArray::fromRawData(reinterpret_cast<const char*> (voiceBin.data()), voiceBin.size());
    QBuffer* readBuff = new QBuffer(&voiceArray);

    readBuff->open(QIODevice::ReadWrite);
    readBuff->seek(0);

    QAudioSink* sink = new QAudioSink(format, this);
    cout<<"trying to start audio: "<<readBuff->data().toInt()<<endl;
    // sink->setVolume(1);
    // test_buff->buffer().
    // readBuff->
    readBuff->seek(0);
    sink->start(readBuff);
}

void Widget::on_mute_unmute_clicked()
{
    if (ui->mute_unmute->text().toStdString() == "Mute")
    {
        source->stop();
        ui->mute_unmute->setText(QString::fromStdString("Unmute"));
    }
    else
    {
        source->start(buffer);
        ui->mute_unmute->setText(QString::fromStdString("Mute"));
    }
}



