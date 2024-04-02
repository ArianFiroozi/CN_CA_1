#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QBuffer>
#include <QAudioSource>

#include "Peer.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Peer* peer;
    QBuffer* buffer;
    QAudioSource* source;

    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_show_sdp_clicked();

    void on_textEdit_textChanged();

    void on_connect_peer_clicked();

    void on_send_text_clicked();

    void on_send_audio_clicked();

    void on_show_text_message_clicked();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
