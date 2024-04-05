/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QTextEdit *textEdit;
    QTextBrowser *textBrowser;
    QPushButton *show_sdp;
    QPushButton *connect_peer;
    QPushButton *send_text;
    QTextEdit *textEdit_2;
    QPushButton *send_audio;
    QTextBrowser *message_display;
    QPushButton *mute_unmute;
    QSlider *sound_control;
    QLabel *label;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(624, 499);
        textEdit = new QTextEdit(Widget);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setGeometry(QRect(30, 30, 271, 161));
        textBrowser = new QTextBrowser(Widget);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        textBrowser->setGeometry(QRect(320, 30, 271, 161));
        show_sdp = new QPushButton(Widget);
        show_sdp->setObjectName(QString::fromUtf8("show_sdp"));
        show_sdp->setGeometry(QRect(320, 200, 271, 51));
        connect_peer = new QPushButton(Widget);
        connect_peer->setObjectName(QString::fromUtf8("connect_peer"));
        connect_peer->setGeometry(QRect(30, 200, 271, 51));
        send_text = new QPushButton(Widget);
        send_text->setObjectName(QString::fromUtf8("send_text"));
        send_text->setGeometry(QRect(30, 320, 271, 61));
        textEdit_2 = new QTextEdit(Widget);
        textEdit_2->setObjectName(QString::fromUtf8("textEdit_2"));
        textEdit_2->setGeometry(QRect(30, 270, 271, 41));
        send_audio = new QPushButton(Widget);
        send_audio->setObjectName(QString::fromUtf8("send_audio"));
        send_audio->setGeometry(QRect(320, 320, 271, 61));
        message_display = new QTextBrowser(Widget);
        message_display->setObjectName(QString::fromUtf8("message_display"));
        message_display->setGeometry(QRect(30, 420, 271, 41));
        mute_unmute = new QPushButton(Widget);
        mute_unmute->setObjectName(QString::fromUtf8("mute_unmute"));
        mute_unmute->setGeometry(QRect(320, 270, 61, 41));
        sound_control = new QSlider(Widget);
        sound_control->setObjectName(QString::fromUtf8("sound_control"));
        sound_control->setGeometry(QRect(390, 280, 201, 21));
        sound_control->setOrientation(Qt::Horizontal);
        label = new QLabel(Widget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(30, 390, 121, 28));

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
        textEdit->setHtml(QCoreApplication::translate("Widget", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Noto Sans Arabic'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">insert sdp</p></body></html>", nullptr));
        textBrowser->setHtml(QCoreApplication::translate("Widget", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Noto Sans Arabic'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">my sdp</p></body></html>", nullptr));
        show_sdp->setText(QCoreApplication::translate("Widget", "Show SDP", nullptr));
        connect_peer->setText(QCoreApplication::translate("Widget", "Connect", nullptr));
        send_text->setText(QCoreApplication::translate("Widget", "send", nullptr));
        textEdit_2->setHtml(QCoreApplication::translate("Widget", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Noto Sans Arabic'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">msg</p></body></html>", nullptr));
        send_audio->setText(QCoreApplication::translate("Widget", "send audio", nullptr));
        mute_unmute->setText(QCoreApplication::translate("Widget", "Mute", nullptr));
        label->setText(QCoreApplication::translate("Widget", "Received Message:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
