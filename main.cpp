#include "widget.h"

#include <QApplication>
#include <QtMultimedia>
#include <QtMultimedia/QMediaPlayer>
#include "rtc/rtc.hpp"
#include "rtc/description.hpp"
#include "rtc/datachannel.hpp"

using namespace std;

void logPc(rtc::PeerConnection *pc)
{
    pc->onIceStateChange([](rtc::PeerConnection::IceState iceState){
        cout<<"icestate changed:"<<iceState<<endl;
    });

    pc->onStateChange([](rtc::PeerConnection::State state) {
        std::cout << "State: " << state << std::endl;
    });

    pc->onGatheringStateChange([](rtc::PeerConnection::GatheringState state) {
        std::cout << "Gathering state: " << state << std::endl;
    });

    pc->onLocalDescription([pc](rtc::Description sdp) {
        // Send the SDP to the remote peer
        // cout<<sdp.generateSdp()<<endl;
        pc->setRemoteDescription(rtc::Description(sdp));
    });

    pc->onLocalCandidate([pc](rtc::Candidate candidate) {
        // Send the candidate to the remote peer
        pc->addRemoteCandidate(rtc::Candidate(candidate.candidate(), candidate.mid()));
    });
}

void logDc(shared_ptr<rtc::DataChannel> dc)
{
    dc->onOpen([]() {
        std::cout << "Open" << std::endl;
    });

    dc->onMessage([](std::variant<rtc::binary, rtc::string> message) {
        if (std::holds_alternative<rtc::string>(message)) {
            std::cout << "Received: " << get<rtc::string>(message) << std::endl;
        }
    });
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;

    rtc::Configuration config;
    config.iceServers.emplace_back("stun:stun.l.google.com:19302");

    rtc::PeerConnection pc(config);
    logPc(&pc);

    cout<<"signaling state:"<<pc.signalingState()<<endl;

    shared_ptr<rtc::DataChannel> dc = pc.createDataChannel("test");
    logDc(dc);

    pc.onDataChannel([&dc](shared_ptr<rtc::DataChannel> incoming) {
        dc = incoming;
        dc->send("Hello world!");
    });

    /// pc1 ///////////////////////////////////

    rtc::PeerConnection pc2(config);
    logPc(&pc2);

    cout<<"signaling state:"<<pc2.signalingState()<<endl;

    shared_ptr<rtc::DataChannel> dc2 = pc2.createDataChannel("test");
    logDc(dc2);

    pc2.onDataChannel([&dc2](shared_ptr<rtc::DataChannel> incoming) {
        dc2 = incoming;
        dc2->send("Hello world!");
    });


    pc.setLocalDescription(rtc::Description::Type::Offer);
    pc2.setLocalDescription(rtc::Description::Type::Answer);

    rtc::Description d = rtc::Description(pc.localDescription().value().generateSdp());
    cout<<pc.localDescription().value().generateSdp()<<endl;
    cout<<pc2.localDescription().value().generateSdp()<<endl;
    pc2.setRemoteDescription(d);

    // w.show();
    return a.exec();
}
