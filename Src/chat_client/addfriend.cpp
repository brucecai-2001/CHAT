#include "addfriend.h"
#include "ui_addfriend.h"

AddFriend::AddFriend(QTcpSocket *s, QString u, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddFriend)
{
    ui->setupUi(this);
    this -> socket =s;
    userName = u;
}

AddFriend::~AddFriend()
{
    delete ui;
}

void AddFriend::on_CancelPb_clicked()
{
    this->close();
}


void AddFriend::on_AddPb_clicked()
{
    QString friendName=ui->lineEdit->text();
    QJsonObject obj;
    obj.insert("cmd","add");
    obj.insert("user",userName);
    obj.insert("friend",friendName);
    //发送请求
    QByteArray ba=QJsonDocument(obj).toJson();
    socket -> write(ba);

    this->close();
}

