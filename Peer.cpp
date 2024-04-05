#include<chrono>
#include<thread>

#include "rtc/rtc.hpp"
#include "rtc/description.hpp"
#include "rtc/datachannel.hpp"
#include "Peer.h"

using namespace std;


void Peer::logPc(rtc::PeerConnection *pc)
{
    pc->onIceStateChange([](rtc::PeerConnection::IceState iceState){
        cout <<"icestate changed: "<<iceState<<endl;
    });

    pc->onSignalingStateChange([](rtc::PeerConnection::SignalingState state){
        cout <<"signaling changed: "<<state<<endl;
    });

    pc->onStateChange([](rtc::PeerConnection::State state) {
        std::cout << "state changed: " << state << std::endl;
    });

    pc->onGatheringStateChange([](rtc::PeerConnection::GatheringState state) {
        std::cout << "gathering state changed: " << state << std::endl;
    });

    pc->onLocalDescription([pc](rtc::Description sdp) {
        pc->setRemoteDescription(rtc::Description(sdp));
    });

    pc->onLocalCandidate([pc](rtc::Candidate candidate) {
        pc->addRemoteCandidate(rtc::Candidate(candidate.candidate(), candidate.mid()));
    });

    pc->onIceStateChange([pc](rtc::PeerConnection::IceState iceState){
        std::cout<< "ice state changed: " << iceState << endl;
    });
}

void Peer::logDc(shared_ptr<rtc::DataChannel> dc)
{
    dc->onOpen([this]() {
        std::cout << "datachannel open" << std::endl;
        state = 1;
    });

    dc->onMessage([this](std::variant<rtc::binary, rtc::string> message) {
        if (std::holds_alternative<rtc::string>(message)) {
            std::cout << "message received: " << get<rtc::string>(message) << std::endl;
            emit variableChanged(QString::fromStdString(get<rtc::string>(message)));
        }
    });
}

Peer::Peer(QObject *parent) : QObject(parent)
{
    state = 0;
    config.iceServers.emplace_back("stun:stun.l.google.com:19302");
    pc = new rtc::PeerConnection(config);
}

Peer::~Peer()
{}

void Peer::printLog()
{
    logPc(pc);
    logDc(dc);
}

void Peer::createDataChannel(std::string name)
{
    dc = pc->createDataChannel(name);

    pc->onDataChannel([this](shared_ptr<rtc::DataChannel> incoming) {
        incomingDc = incoming;
        incomingDc->send("peer is connected");
    });

    pc->onTrack([this](std::shared_ptr<rtc::Track> rec_track){
        auto session = std::make_shared<rtc::RtcpReceivingSession>();
        rec_track->setMediaHandler(session);
        rtc::Description::Media desc = rec_track->description();
        desc.setDirection(rtc::Description::Direction::SendRecv);

        cout<<"got track: "<<rec_track->isOpen()<<endl;
        cout<<rec_track->description().generateSdp();

        rec_track = this->pc->addTrack(desc);

        receive_track = rec_track;

        receive_track->onOpen([this]() {
            cout<<"track opened: "<<receive_track->direction()<<endl;
        });

        receive_track->onMessage(
            [this](rtc::binary message) {
                cout<<"in receive: binary message recieved"<<endl;
                emit voiceReceived(message);
            },
            [](rtc::string msg){
                cout<<"msgstr: "<<msg<<endl;
            });
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
