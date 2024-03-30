#ifndef PEER_H
#define PEER_H
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

    void logPc(rtc::PeerConnection *pc, int id);
    void logDc(shared_ptr<rtc::DataChannel> dc, int id);

public:
    rtc::PeerConnection* pc;
    Peer(int _id);
    void printLog();
    void createDataChannel(std::string name);
    void sendMsg(std::string msg);
    void setLocalToOffer();
    rtc::Description getSdp();
    void setRemoteDescription(rtc::Description sdp);
    int isConnected();
};

#endif // PEER_H
