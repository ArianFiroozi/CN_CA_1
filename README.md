# CN_CA1 - Arian Firoozi - 810100196
## About the Project

In this project we want to create a QT application using WebRTC that allows users to communicate with audio calls. Connection type used to stablish communication is p2p and is implemented with libdatachannel library which provides WebRTC for c++.
Although the goal was to implement audio calls, audio packets sent through tracks did not play the audio so this code only successfuly transfers text messages between peers.

## User Interface

UI of this application consists of two text boxes that one shows SDP of the current peer and the other is used to get SDP of other peer via user input. First peer wanting to communicate should get its own SDP and somehow transfer it to other peer. SDP is the sould be entered in the other peer's text box and connect button should be pressed. This sets the remote description of peer B as local description of peer A. Necessary datachannel and tracks to send and receive is created by second peer and only then, peer B should get its own sdp and transfeer it back to peer A. Peer A sets its own remote description as local description of peer B and connection is made. Stun server used for connection is Google's `stun:stun.l.google.com:19302` server.

![image](https://github.com/chosterm/CN_CA_1/assets/126232660/c2a6a491-8db1-45f7-b4f6-8f5112b55c92)

Both users should wait until `peer is connected` message to show up which indicate that everything is set up and they can start communicating. Messages can be sent by typing it in the Msg textbox and pressing send button.

![image](https://github.com/chosterm/CN_CA_1/assets/126232660/44d42020-5954-4cbe-a2bf-a2bd46e436d2)
This project is only tested in local device since I did not have a teammate, but it should work fine for two devices since STUN servers are implemented.

## Audio Transfer

Using QAudioSource, the audio is received from user microphone, and using the Mute button microphon can mutted and unmutted on command. This part works fine as the microphone icon appears by OS when send audio is clicked and disappears when mute is pressed; the other peer gets the message and tries to play it but no audio is played which is a bug that I could not fix until the deadline.

## WebRTC

WebRTC, short for Web Real-Time Communications is a project that helps building real time communications between devices. It can be used to transfer data between web browsers and applications with JavaScript language, however it is also available for c++ and python.
Peer to peer communication in this library is done by following steps:

1. `Signaling`: peers that want to communicate with eachother should first exchange information with a signaling server which then provides the peer with information needed to communicate with other peers.
2. `ICE`: Interactive Connectivity Establishment is a protocol that uses STUN and/or TURN servers to allow communication between peers.
  2.1 `STUN`: is a protocol to get one's public IP address and restrictions. A STUN server will provide information about public IP and router's NAT which is another protocol that translates public IP to private IP.
  2.2 `TURN`: if symmetric NAT restriction is applied, peers can not communicate directly, therefor some server is needed to get data from one peer and forward it to the other. This server is called TURN server.
3. `Communicating`: once connection is stablished via signaling server and STUN/TURN servers, two peers communicate either directly or only one server in between them, depending on NAT restrictions.

Its worth mentioning that multimedia content of the connection is exchanged with Session Description Protocol(SDP) which is a format to describe metadata used by a device.

this is diagram of basic p2p communication from techtarget.com:
![image](https://github.com/chosterm/CN_CA_1/assets/126232660/88ca84ee-0c02-4f18-b6ed-a54f7908f757)

### Pros:
P2P communication is much faster than traditional way of routing and using servers.
WebRTC is supported by most of the web browsers.
Communication works regardless of the operating system as long as it supports WebRTC.
WebRTC is open source.

### Cons:
WebRTC can consume large amounts of bandwidth and for users with inadequate bandwidth, connection can be distrupted.
Signaling has no standard protocol and it only depends on implementation.
Security protocols for metadata and signaling should be specified by developers.

## Resources:
https://github.com/paullouisageneau/libdatachannel
https://www.techtarget.com/searchunifiedcommunications/definition/WebRTC-Web-Real-Time-Communications
https://trtc.io/blog/details/what-is-webrtc
https://developer.mozilla.org/en-US/docs/Web/API/WebRTC_API
