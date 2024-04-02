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
    buffer = new QBuffer();

    QAudioFormat format;
    format.setSampleRate(8000);
    format.setChannelCount(1);
    format.setSampleFormat(QAudioFormat::UInt8);
    source = new QAudioSource(format, this);
    cout<<"buffer opened:"<<buffer->open(QIODevice::ReadWrite)<<endl;
    cout<<"buffer state"<<buffer->isOpen()<<endl;
    source->start(buffer);

    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_show_sdp_clicked()
{

    if (ui->sender_radio->isChecked()){
        const rtc::SSRC ssrc = 42;
        rtc::Description::Audio media("audio", rtc::Description::Direction::SendRecv);
        media.addSSRC(ssrc, "audio-send");

        peer->send_track = peer->pc->addTrack(media);
        peer->send_track->onOpen([]() {
            cout<<"send track opened"<<endl;
        });

        peer->send_track->onOpen([this](){
            cout<<"send track is open, trying to send smth"<<endl;
            peer->send_track->send("daaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
        });


        peer->send_track->onMessage([](std::variant<rtc::binary, rtc::string> message) {
            cout<<"received from send!"<<endl;
        });
        /////
        rtc::Description::Audio rec_media("audio", rtc::Description::Direction::SendRecv);
        rec_media.addSSRC(ssrc, "audio-rec");

        peer->receive_track = peer->pc->addTrack(rec_media);
        peer->receive_track->onOpen([]() {
            cout<<"rec track opened"<<endl;
        });

        peer->receive_track->onOpen([this](){
            cout<<"rec track is open"<<endl;
        });


        peer->receive_track->onMessage(
            [this](rtc::binary message) {
                cout<<"msgbin"<<endl;
                cout<< reinterpret_cast<const char *>(message.data())<<endl;
            },
            [](rtc::string msg){
                cout<<"msgstr"<<msg<<endl;
            });

    }


    peer->createDataChannel("mydc");
    peer->setLocalToOffer();

    if (ui->receiver_radio->isChecked()){ ////?
        const rtc::SSRC ssrc = 42;
        rtc::Description::Audio rec_media("audio", rtc::Description::Direction::SendRecv);
        rec_media.addSSRC(ssrc, "audio-receive");

        peer->send_track = peer->pc->addTrack(rec_media);
        auto session = std::make_shared<rtc::RtcpReceivingSession>();
        peer->send_track->setMediaHandler(session);

        // peer->send_track->send("SRGGGEWrgwermmmmmmmmmmmmmmmmmmmmmmmmmmgergr");
        // peer->receive_track->onMessage([this] (std::variant<rtc::binary, rtc::string> message) {
        //     cout<<"i got msg"<<endl;
        //     if (std::holds_alternative<rtc::binary>(message)) {
        //         rtc::binary binaryMessage = std::get<rtc::binary>(message);
        //         QByteArray audioData(reinterpret_cast<char*>(binaryMessage.data()), binaryMessage.size());

        //         buffer->write(audioData);
        //     }
        // });
    }

    string mySdp = peer->getSdp();
    ui->textBrowser->setText(QString::fromUtf8(mySdp.c_str()));
    peer->printLog();
}


void Widget::on_textEdit_textChanged()
{
}


void Widget::on_connect_peer_clicked()
{
    peer->setRemoteDescription(ui->textEdit->toPlainText().toStdString());
}

void Widget::on_send_text_clicked()
{
    peer->sendMsg(ui->textEdit_2->toPlainText().toStdString());
}

void Widget::on_send_audio_clicked()
{
    if (ui->sender_radio->isChecked() || true){
        // while(1){
            // std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            cout<<"buffer opened:"<<buffer->open(QIODevice::ReadWrite)<<endl;
            QByteArray audioData = buffer->readAll();
            if (!audioData.isEmpty()) {
                cout<<"sending audio!"<<audioData.size()<<endl;
                std::string rtpPacket(audioData.begin(), audioData.begin() + 200);

                if (peer->send_track->isOpen())
                    cout<<"send is:"<<peer->send_track->send(std::move(rtpPacket));
                else
                    cout<<"track not open"<<endl;
                buffer->buffer().clear();
            }
        // }
    }
    else{
        // rtc::Description::Audio media("audio", rtc::Description::Direction::RecvOnly);

        // peer->receive_track = peer->pc->addTrack(media);
        // auto session = std::make_shared<rtc::RtcpReceivingSession>();
        // peer->receive_track->setMediaHandler(session);

        // peer->receive_track->onMessage([this] (std::variant<rtc::binary, rtc::string> message) {
        //     cout<<"i got msg"<<endl;
        //     if (std::holds_alternative<rtc::binary>(message)) {
        //         rtc::binary binaryMessage = std::get<rtc::binary>(message);
        //         QByteArray audioData(reinterpret_cast<char*>(binaryMessage.data()), binaryMessage.size());

        //         buffer->write(audioData);
        //     }
        // });
    }
}

void Widget::on_show_text_message_clicked()
{

    ui->message_display->setText(QString::fromStdString(peer->last_received_msg));
}

