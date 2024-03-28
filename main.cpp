#include "widget.h"

#include <QApplication>
#include <QtMultimedia>
#include <QtMultimedia/QMediaPlayer>

#include<chrono>
#include<thread>

#include "rtc/rtc.hpp"
#include "rtc/description.hpp"
#include "rtc/datachannel.hpp"

using namespace std;

class Peer{
private:
    int id;
    rtc::Configuration config;
    shared_ptr<rtc::DataChannel> dc;
    shared_ptr<rtc::DataChannel> incomingDc;
    int state;

    void logPc(rtc::PeerConnection *pc, int id)
    {
        pc->onIceStateChange([id](rtc::PeerConnection::IceState iceState){
            cout << id <<" icestate changed: "<<iceState<<endl;
        });

        pc->onSignalingStateChange([id](rtc::PeerConnection::SignalingState state){
            cout << id <<" signaling changed: "<<state<<endl;
        });

        pc->onStateChange([id](rtc::PeerConnection::State state) {
            std::cout << id << " state changed: " << state << std::endl;
        });

        pc->onGatheringStateChange([id](rtc::PeerConnection::GatheringState state) {
            std::cout << id << " gathering state changed: " << state << std::endl;
        });

        pc->onLocalDescription([pc](rtc::Description sdp) {
            // cout << id<<sdp.generateSdp()<<endl;
            pc->setRemoteDescription(rtc::Description(sdp));
        });

        pc->onLocalCandidate([pc](rtc::Candidate candidate) {
            pc->addRemoteCandidate(rtc::Candidate(candidate.candidate(), candidate.mid()));
        });
    }

    void logDc(shared_ptr<rtc::DataChannel> dc, int id)
    {
        dc->onOpen([id, this]() {
            std::cout << id << " datachannel open" << std::endl;
            state = 1; //shouldn't be here
        });

        dc->onMessage([id](std::variant<rtc::binary, rtc::string> message) {
            if (std::holds_alternative<rtc::string>(message)) {
                std::cout << id << " message received: " << get<rtc::string>(message) << std::endl;
            }
        });
    }

public:
    rtc::PeerConnection* pc; //should be private in final

    Peer(int _id)
    {
        id = _id;
        state = 0; //TODO enum
        config.iceServers.emplace_back("stun:stun.l.google.com:19302");
        pc = new rtc::PeerConnection(config);
    }

    void printLog()
    {
        logPc(pc, id);
        logDc(dc, id);
    }

    void createDataChannel(std::string name)
    {
        dc = pc->createDataChannel(name);

        pc->onDataChannel([this](shared_ptr<rtc::DataChannel> incoming) {
            incomingDc = incoming;
            incomingDc->send(to_string(id) + " is connected");
        });
    }

    void sendMsg(std::string msg)
    {
        incomingDc->send(msg);
    }

    void setLocalToOffer()
    {
        pc->setLocalDescription(rtc::Description::Type::Offer);
    }

    rtc::Description getSdp()
    {
        return pc->localDescription().value().generateSdp();
    }

    void setRemoteDescription(rtc::Description sdp) {
        pc->setRemoteDescription(sdp);
    }

    int isConnected(){
        return state;
    }
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;

    // rtcInitLogger(rtcLogLevel::RTC_LOG_DEBUG, NULL);

    Peer firstPeer(1);
    Peer secondPeer(2);

    firstPeer.createDataChannel("recieve");
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


    w.show();
    return a.exec();
}
