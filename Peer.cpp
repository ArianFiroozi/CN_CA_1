#include<chrono>
#include<thread>

#include "rtc/rtc.hpp"
#include "rtc/description.hpp"
#include "rtc/datachannel.hpp"
#include "Peer.h"
#include "widget.h"

using namespace std;


void Peer::logPc(rtc::PeerConnection *pc, int id)
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

void Peer::logDc(shared_ptr<rtc::DataChannel> dc, int id)
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

Peer::Peer(int _id)
{
    id = _id;
    state = 0; //TODO enum
    config.iceServers.emplace_back("stun:stun.l.google.com:19302");
    pc = new rtc::PeerConnection(config);
}

void Peer::printLog()
{
    logPc(pc, id);
    logDc(dc, id);
}

void Peer::createDataChannel(std::string name)
{
    dc = pc->createDataChannel(name);

    pc->onDataChannel([this](shared_ptr<rtc::DataChannel> incoming) {
        incomingDc = incoming;
        incomingDc->send(to_string(id) + " is connected");
    });
}

void Peer::sendMsg(std::string msg)
{
    incomingDc->send(msg);
}

void Peer::setLocalToOffer()
{
    pc->setLocalDescription(rtc::Description::Type::Offer);
}

rtc::Description Peer::getSdp()
{
    return pc->localDescription().value().generateSdp();
}

void Peer::setRemoteDescription(rtc::Description sdp) {
    pc->setRemoteDescription(sdp);
}

int Peer::isConnected(){
    return state;
}
