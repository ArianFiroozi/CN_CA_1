#include "widget.h"

#include <QApplication>
#include <QtMultimedia>
#include <QtMultimedia/QMediaPlayer>
#include "rtc/rtc.hpp"
#include "rtc/description.hpp"
#include "rtc/datachannel.hpp"

using namespace std;

void logPc(rtc::PeerConnection *pc, int id)
{
    pc->onIceStateChange([id](rtc::PeerConnection::IceState iceState){
        cout << id <<"icestate changed:"<<iceState<<endl;
    });

    pc->onStateChange([id](rtc::PeerConnection::State state) {
        std::cout << id << "State: " << state << std::endl;
    });

    pc->onGatheringStateChange([id](rtc::PeerConnection::GatheringState state) {
        std::cout << id << "Gathering state: " << state << std::endl;
    });

    pc->onLocalDescription([pc](rtc::Description sdp) {
        // Send the SDP to the remote peer
        // cout << id<<sdp.generateSdp()<<endl;
        pc->setRemoteDescription(rtc::Description(sdp));
    });

    pc->onLocalCandidate([pc](rtc::Candidate candidate) {
        // Send the candidate to the remote peer
        pc->addRemoteCandidate(rtc::Candidate(candidate.candidate(), candidate.mid()));
    });
}

void logDc(shared_ptr<rtc::DataChannel> dc, int id)
{
    dc->onOpen([id]() {
        std::cout << id << "datachannel Open" << std::endl;
    });

    dc->onMessage([id](std::variant<rtc::binary, rtc::string> message) {
        if (std::holds_alternative<rtc::string>(message)) {
            std::cout << id << "message received: " << get<rtc::string>(message) << std::endl;
        }
    });
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;

    // rtcInitLogger(rtcLogLevel::RTC_LOG_DEBUG, NULL);

    rtc::Configuration config;
    config.iceServers.emplace_back("stun:stun.l.google.com:19302");

    rtc::PeerConnection pc(config);
    logPc(&pc, 1);

    cout<<"signaling state:"<<pc.signalingState()<<endl;

    shared_ptr<rtc::DataChannel> dc = pc.createDataChannel("test");
    logDc(dc, 1);

    // pc.onDataChannel([&dc](shared_ptr<rtc::DataChannel> incoming) {
    //     dc = incoming;
    //     dc->send("Hello world!");
    // });

    /// pc1 ///////////////////////////////////

    rtc::PeerConnection pc2(config);
    logPc(&pc2, 2);

    cout<<"signaling state:"<<pc2.signalingState()<<endl;


    pc.setLocalDescription(rtc::Description::Type::Offer);

    rtc::Description d = rtc::Description(pc.localDescription().value().generateSdp());
    // cout<<pc.localDescription().value().generateSdp()<<endl;
    // cout<<pc2.localDescription().value().generateSdp()<<endl;
    pc2.setRemoteDescription(d);

    shared_ptr<rtc::DataChannel> dc2 = pc2.createDataChannel("test2");
    // logDc(dc2, 2);

    pc2.onDataChannel([&dc2](shared_ptr<rtc::DataChannel> incoming) {
        dc2 = incoming;
        dc2->send("Hello world!");
    });

    pc2.setLocalDescription(rtc::Description::Type::Offer); //?

    pc.setRemoteDescription(pc2.localDescription().value().generateSdp());

    // rtc::Description::Media m("hello", "mid");
    // pc.addTrack(m);
    // rtcSendMessage(1, "hello fuckers", 100);

    w.show();
    return a.exec();
}
