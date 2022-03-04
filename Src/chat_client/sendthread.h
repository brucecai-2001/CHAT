#ifndef SENDTHREAD_H
#define SENDTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QHostAddress>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>

class SendThread : public QThread
{
public:
    SendThread(QJsonObject obj);
    void run(); //线程开始运行时调用的函数

private:
    QString fileName;
    int     fileLength;
    int     port;
};

#endif // SENDTHREAD_H
