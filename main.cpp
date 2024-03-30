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

    Peer firstPeer(1);
    Peer secondPeer(2);

    QIODevice* send_device = new QBuffer(); //?
    send_device->open(QIODevice::ReadWrite);

    // QMediaCaptureSession* session = new QMediaCaptureSession();
    // QAudioInput* audioInput = new QAudioInput(send_device);
    // QMediaRecorder* recorder = new QMediaRecorder();
    // QAudioOutput* audioOutput = new QAudioOutput();

    QAudioFormat format;
    // Set up the format, eg.
    format.setSampleRate(8000);
    format.setChannelCount(1);
    format.setSampleFormat(QAudioFormat::UInt8);

    QAudioSource* source = new QAudioSource();
    // connect(source, &QAudioSource::stateChanged, &audioInput::handleStateChanged);

    source->start(send_device);

    // session->setAudioInput(audioInput);
    // session->setRecorder(recorder);
    // session->setAudioOutput(audioOutput); //uncomment for local audio test
    // // session->connectNotify();
    // recorder->record();
    // audioInput->device().
    // audioOutput->setMuted(true);

    // QIODevice* device = audio->start();
    // QAudioInput* audio = new QAudioInput(device);
    const rtc::SSRC ssrc = 42;
    rtc::Description::Audio media("audio", rtc::Description::Direction::SendOnly);
    media.addSSRC(ssrc, "audio-send");

    auto track = firstPeer.pc->addTrack(media
        // rtc::Description::Media("audio 9 UDP/TLS/RTP/SAVPF 0\nc=IN IP4 0.0.0.0\na=rtpmap:0 PCMU/8000\na=mid:audio\na=sendrecv\na=rtcp:9 IN IP4 0.0.0.0\n")
        ); //?

///////////////////////////////////////////

    QBuffer* buffer = new QBuffer();
    buffer->open(QIODevice::ReadWrite);

    std::shared_ptr<rtc::Track> receive_track;

    secondPeer.pc->onTrack([buffer, &receive_track](std::shared_ptr<rtc::Track> rec_track){
        cout<<"i got track"<<rec_track->isOpen()<<endl;
        receive_track = rec_track;
    });

    // audio_rec->start(buffer);

    ///////////////// DO NOT TOUCH /////////////////////////////

    firstPeer.createDataChannel("receive");
    firstPeer.setLocalToOffer();

    secondPeer.setRemoteDescription(firstPeer.getSdp());
    secondPeer.createDataChannel("send");
    secondPeer.setLocalToOffer();
    firstPeer.setRemoteDescription(secondPeer.getSdp());

    firstPeer.printLog();
    secondPeer.printLog();

    while(!firstPeer.isConnected() || !secondPeer.isConnected()) //wait till connect
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    firstPeer.sendMsg("first peer communicating");
    secondPeer.sendMsg("second peer communicating");

    /////////////////////////////////////////////////////////////////////////

    // auto rec_track = secondPeer.pc->addTrack(rec_media);

    // auto rec_session = std::make_shared<rtc::RtcpReceivingSession>();
    // rec_track->setMediaHandler(rec_session);

    // rec_track->onMessage([](rtc::binary message) {
    //         cout<<"rec Track got msg"<<endl;
    //         // This is an RTP packet
    //         cout<<message.data();
    //     },
    //     nullptr);

    // char buffer[2048] = "dsffffffffffffffffffffffffffffvvfffffffffffffffffffffffffffssssssssssssssssssssss";
    // auto rtp = reinterpret_cast<rtc::RtpHeader *>(buffer);
    // rtp->setSsrc(ssrc);
    // track->send(reinterpret_cast<const std::byte *>(buffer), sizeof(buffer));

    cout<<receive_track->isOpen()<<endl;
    receive_track->onMessage([buffer] (std::variant<rtc::binary, rtc::string> message) {
        cout<<"i got msg"<<endl;
        if (std::holds_alternative<rtc::binary>(message)) {
            // Convert the binary data to a QByteArray
            rtc::binary binaryMessage = std::get<rtc::binary>(message);
            QByteArray audioData(reinterpret_cast<char*>(binaryMessage.data()), binaryMessage.size());

            // Write the audio data to the buffer
            buffer->write(audioData);
        }
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    for (int i=0;i<10;i++) {
        char data[1024];
        cout<<send_device->read(data,1024)<<endl;
        cout<<data<<endl;

        QByteArray audioData = QByteArray("fewrsdgsfwdefrggdddwerdwefrvfffffffffffffffffffffffffffffr"); //= send_device->readAll();
        // cout<<"empty?"<<endl;
        if (!audioData.isEmpty()) {
            cout<<"NOT EMPTY!"<<endl;
            // Convert the QByteArray to std::string
            std::string rtpPacket(audioData.begin(), audioData.end());

            // Send the RTP packet
            cout<<"send is:"<<track->send(std::move(rtpPacket));
        }
    }

    w.show();
    return a.exec();
}
