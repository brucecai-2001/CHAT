#ifndef PRIVATE_CHAT_H
#define PRIVATE_CHAT_H

#include <QWidget>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QList>
#include <QCloseEvent>
#include <QFileDialog>
#include <QCoreApplication>
#include <QFile>

struct ChatInfo;
class Chatlist;
#include "chatlist.h"

namespace Ui {
class Private_Chat;
}

class Private_Chat : public QWidget
{
    Q_OBJECT

public:
    explicit Private_Chat(QTcpSocket *s,QString u, QString f,Chatlist *c, QList<ChatInfo> *wl, QWidget *parent = nullptr);
    ~Private_Chat();

    void closeEvent(QCloseEvent *event);//关闭窗口

private slots:
    void on_SendPb_clicked();
    void receive_text(QJsonObject);
    void on_FilePb_clicked();

private:
    Ui::Private_Chat *ui;
    QTcpSocket *socket;
    QString userName;
    QString friendName;
    Chatlist *mainWidget;
    QList<ChatInfo> *widgetList;
};

#endif // PRIVATE_CHAT_H
