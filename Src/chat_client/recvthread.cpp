#include "recvthread.h"

RecvThread::RecvThread(QJsonObject obj)
{
    total=0;
    port = obj.value("port").toInt();
    fileLength = obj.value("length").toInt();

    QString pathName = obj.value("filename").toString();
    QStringList strlist = pathName.split('/');
    fileName = strlist.at(strlist.size()-1);
}

void RecvThread::run()
{
    file = new QFile(fileName); //在.exe上一级目录创建文件
    file->open(QIODevice::WriteOnly);

    recvSocket = new QTcpSocket;
    connect(recvSocket, &QTcpSocket::readyRead, this, &RecvThread::recv_file, Qt::DirectConnection); //将recv_file连接到run线程上
    recvSocket->connectToHost(QHostAddress("8.208.11.15"),port);
    if(recvSocket->waitForConnected(10000)){
        this->quit();
    }
    else{

    }

    exec();
}

void RecvThread::recv_file()
{
    QByteArray ba = recvSocket->readAll();
    total += ba.size();
    file->write(ba);
    if(total >= fileLength){
        file->close();
        recvSocket->close();
        delete recvSocket;
        this->quit();
    }

}
