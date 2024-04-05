# CN_CA1 - Arian Firoozi - 810100196
## About the Project

In this project we want to create a QT application using WebRTC that allows users to communicate with audio calls. Connection type used to stablish communication is p2p and is implemented with libdatachannel library which provides WebRTC for c++.
Although the goal was to implement audio calls, audio packets sent through tracks did not play the audio so this code only successfuly transfers text messages between peers.

## User Interface

UI of this application consists of two text boxes that one shows SDP of the current peer and the other is used to get SDP of other peer via user input. First peer wanting to communicate should get its own SDP and somehow transfer it to other peer. SDP is the sould be entered in the other peer's text box and connect button should be pressed. This sets the remote description of peer B as local description of peer A. Necessary datachannel and tracks to send and receive is created by second peer and only then, peer B should get its own sdp and transfeer it back to peer A. Peer A sets its own remote description as local description of peer B and connection is made. Stun server used for connection is Google's stun:stun.l.google.com:19302 server.

![image](https://github.com/chosterm/CN_CA_1/assets/126232660/c2a6a491-8db1-45f7-b4f6-8f5112b55c92)

Both users should wait until 'peer is connected' message to show up which indicate that everything is set up and they can start communicating. Messages can be sent by typing it in the Msg textbox and pressing send button.

![image](https://github.com/chosterm/CN_CA_1/assets/126232660/44d42020-5954-4cbe-a2bf-a2bd46e436d2)
