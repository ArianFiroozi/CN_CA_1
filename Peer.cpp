#include<chrono>
#include<thread>

#include "rtc/rtc.hpp"
#include "rtc/description.hpp"
#include "rtc/datachannel.hpp"
#include "Peer.h"

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

    pc->onIceStateChange([pc, id](rtc::PeerConnection::IceState iceState){
        std::cout<< id << " ice state changed: " << iceState << endl;
    });
}

void Peer::logDc(shared_ptr<rtc::DataChannel> dc, int id)
{
    dc->onOpen([id, this]() {
        std::cout << id << " datachannel open" << std::endl;
        state = 1; //shouldn't be here
    });

    dc->onMessage([id,this](std::variant<rtc::binary, rtc::string> message) {
        if (std::holds_alternative<rtc::string>(message)) {
            std::cout << id << " message received: " << get<rtc::string>(message) << std::endl;
            emit variableChanged(QString::fromStdString(get<rtc::string>(message)));
        }
    });
}

Peer::Peer(int _id, QObject *parent) : QObject(parent)
{
    id = _id;
    state = 0; //TODO enum
    config.iceServers.emplace_back("stun:stun.l.google.com:19302");
    pc = new rtc::PeerConnection(config);
}

Peer::~Peer()
{}

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

    pc->onTrack([this](std::shared_ptr<rtc::Track> rec_track){
        auto session = std::make_shared<rtc::RtcpReceivingSession>();
        rec_track->setMediaHandler(session);
        rtc::Description::Media desc = rec_track->description();
        desc.setDirection(rtc::Description::Direction::SendRecv);

        cout<<id<<" got track "<<rec_track->isOpen()<<endl;

        cout<<rec_track->description().generateSdp();
        rec_track = this->pc->addTrack(desc);

        receive_track = rec_track;

        receive_track->onOpen([this]() {
            cout<<"track opened: "<<receive_track->direction()<<endl;
        });

        receive_track->onMessage(
            [this](rtc::binary message) {
                cout<<"in receive: binary message recieved"<<endl;
                // cout<< reinterpret_cast<const char *>(message.data())<<endl;
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
