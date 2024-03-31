#include "widget.h"

#include <QApplication>
#include <QtMultimedia>
#include <QtMultimedia/QMediaPlayer>

#include<chrono>
#include<thread>

#include "rtc/rtc.hpp"
#include "rtc/description.hpp"
#include "rtc/datachannel.hpp"

#include "Peer.h"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;

    // rtcInitLogger(rtcLogLevel::RTC_LOG_DEBUG, NULL); //uncomment for detailed log

    // Peer firstPeer(1);
    // Peer secondPeer(2);

    // QFile* send_device = new QFile("/home/arian/Desktop/test.raw");
    // cout<<"file opened:"<<send_device->open(QIODevice::ReadWrite)<<endl;

    // QAudioFormat format;

    // format.setSampleRate(8000);
    // format.setChannelCount(1);
    // format.setSampleFormat(QAudioFormat::UInt8);

    // QAudioSource* source = new QAudioSource(format, &a);
    // source->start(send_device);

    // const rtc::SSRC ssrc = 42;
    // rtc::Description::Audio media("audio", rtc::Description::Direction::SendOnly);
    // media.addSSRC(ssrc, "audio-send");

    // auto track = firstPeer.pc->addTrack(media); //?

    // ///////////////////////////////////////////

    // QBuffer* buffer = new QBuffer();
    // buffer->open(QIODevice::ReadWrite);

    // std::shared_ptr<rtc::Track> receive_track;

    // secondPeer.pc->onTrack([buffer, &receive_track](std::shared_ptr<rtc::Track> rec_track){
    //     cout<<"i got track"<<rec_track->isOpen()<<endl;
    //     receive_track = rec_track;
    // });

    // ///////////////// DO NOT TOUCH /////////////////////////////

    // firstPeer.createDataChannel("receive");
    // firstPeer.setLocalToOffer();

    // secondPeer.setRemoteDescription(firstPeer.getSdp());
    // secondPeer.createDataChannel("send");
    // secondPeer.setLocalToOffer();
    // firstPeer.setRemoteDescription(secondPeer.getSdp());
    // secondPeer.printLog();
    // firstPeer.printLog();
    // while(!firstPeer.isConnected() || !secondPeer.isConnected()) //wait till connect
    // {
    //     std::this_thread::sleep_for(std::chrono::milliseconds(100));
    // }
    // firstPeer.sendMsg("first peer communicating");
    // secondPeer.sendMsg("second peer communicating");

    // /////////////////////////////////////////////////////////////////////////

    // auto rec_track = secondPeer.pc->addTrack(media);

    // auto rec_session = std::make_shared<rtc::RtcpReceivingSession>();
    // rec_track->setMediaHandler(rec_session);

    // rec_track->onMessage([](rtc::binary message) {
    //         cout<<"rec Track got msg"<<endl;
    //         cout<<message.data();
    //     },
    //     nullptr);

    // cout<<receive_track->isOpen()<<endl;
    // receive_track->onMessage([buffer] (std::variant<rtc::binary, rtc::string> message) {
    //     cout<<"i got msg"<<endl;
    //     if (std::holds_alternative<rtc::binary>(message)) {
    //         rtc::binary binaryMessage = std::get<rtc::binary>(message);
    //         QByteArray audioData(reinterpret_cast<char*>(binaryMessage.data()), binaryMessage.size());

    //         buffer->write(audioData);
    //     }
    // });

    // std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    // for (int i=0;i<1;i++) {
    //     QByteArray audioData = send_device->readAll();
    //     if (!audioData.isEmpty()) {
    //         cout<<"NOT EMPTY!"<<source->bufferSize()<<endl;
    //         std::string rtpPacket(audioData.begin(), audioData.end());
    //         cout<<"send is:"<<track->send(std::move(rtpPacket));
    //     }
    // }

    w.show();
    return a.exec();
}
