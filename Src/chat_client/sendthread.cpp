#include "sendthread.h"

SendThread::SendThread(QJsonObject obj)
{
    port = obj.value("port").toInt();
    fileName = obj.value("filename").toString();
    fileLength = obj.value("length").toInt();
}

void SendThread::run()
{
    QTcpSocket sendSocket; //发送套接字
    sendSocket.connectToHost(QHostAddress("8.208.11.15"),port);
    if(!sendSocket.waitForConnected(10000)){ //超过十秒没有连接
        this->quit();
    }
    else{
        QFile file(fileName);
        file.open(QIODevice::ReadOnly);
        while(1){
            QByteArray ba = file.read(1024); //一次读1024个字节
            if(ba.size()==0) break;
            sendSocket.write(ba); //发送文件
            sendSocket.flush();
        }

        //关闭文件
        file.close();
        sendSocket.close();
        this->quit();
    }
}
