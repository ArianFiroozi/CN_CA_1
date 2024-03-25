#include "widget.h"
#include "ui_widget.h"

#include <QtMultimedia>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QAudioInput>
#include <QIODevice>
#include <QAudio>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_play_button_clicked()
{
    static QMediaPlayer *player = new QMediaPlayer;
    static QAudioOutput *audioOutput = new QAudioOutput;

    if (!player->hasAudio())
    {
        player->setAudioOutput(audioOutput);
        player->setSource(QUrl::fromLocalFile("/home/arian/Desktop/samplein.wav"));
        audioOutput->setVolume(50);
    }

    if (!player->isPlaying())
    {
        player->play();
    }
    else
    {
        player->pause();
    }
}


void Widget::on_pushButton_clicked()
{
    QMediaCaptureSession* session = new QMediaCaptureSession();
    QAudioInput* audioInput = new QAudioInput();
    QMediaRecorder* recorder = new QMediaRecorder();
    session->setAudioInput(audioInput);
    session->setRecorder(recorder);
    recorder->setMediaFormat(QMediaFormat::Wave);
    recorder->setOutputLocation(QUrl::fromLocalFile("/home/arian/Desktop/samplein"));
    recorder->setAudioSampleRate(48000);
    recorder->setAudioChannelCount(1);
    recorder->record();
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), recorder, SLOT(stop()));
    timer->start(5000);
}

