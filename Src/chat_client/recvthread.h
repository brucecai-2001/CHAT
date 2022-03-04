#ifndef RECVTHREAD_H
#define RECVTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QHostAddress>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>

class RecvThread : public QThread
{
     Q_OBJECT
public:
    RecvThread(QJsonObject obj);
    void run();

private slots:
    void recv_file();

private:
    QString pathName;
    QString fileName;
    int     fileLength;
    int     port;
    int     total;
    QTcpSocket *recvSocket;
    QFile *file;
};

#endif // RECVTHREAD_H
