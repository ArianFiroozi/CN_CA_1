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
    w.setWindowTitle("CN CA1");
    // uncomment for detailed log
    // rtcInitLogger(rtcLogLevel::RTC_LOG_DEBUG, NULL);
    w.show();
    return a.exec();
}
