#include "group_chat.h"
#include "ui_group_chat.h"

Group_chat::Group_chat(QTcpSocket *s,QString u,QString g,Chatlist *c, QList<GroupInfo> *gl,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Group_chat)
{
    ui->setupUi(this);
    socket=s;
    userName=u;
    groupName=g;
    mainWidget=c;
    GroupWidgetList=gl;

    connect(mainWidget, &Chatlist::signal_to_Group_widget, this, &Group_chat::Group_member);
    connect(mainWidget, &Chatlist::signal_to_Group_Widget_chat, this, &Group_chat::show_chat_text);

    //请求好友列表
    QJsonObject obj;
    obj.insert("cmd","get_group_member");
    obj.insert("group",groupName);

    QByteArray ba=QJsonDocument(obj).toJson();
    socket->write(ba);
}


void Group_chat::Group_member(QJsonObject obj)
{
    if(obj.value("cmd").toString() =="get_group_member_reply"){
        if(obj.value("group").toString() == groupName){
            QStringList strList=obj.value("member").toString().split("|");
            for(int i=0;i<strList.size();i++){
                ui->listWidget->addItem(strList.at(i)); //显示群成员
            }
        }
    }
}

void Group_chat::on_SendPb_clicked() //发送消息
{
    QString text = ui->lineEdit->text();
    QJsonObject obj;
    obj.insert("cmd","group_chat");
    obj.insert("user",userName);
    obj.insert("group",groupName);
    obj.insert("text",text);

    QByteArray ba = QJsonDocument(obj).toJson();
    socket->write(ba);

    ui->lineEdit->clear();
    ui->textEdit->append(text);
    ui->textEdit->append("\n");
}

void Group_chat::show_chat_text(QJsonObject obj) //接受消息
{
    if(obj.value("cmd").toString() == "group_chat"){
        if(obj.value("group").toString() == groupName){
            if(this->isMinimized()){
                this->showNormal();
            }
            this->activateWindow();
            ui->textEdit->append(obj.value("text").toString());
            ui->textEdit->append("\n");
        }
    }
}


//关闭窗口
void Group_chat::closeEvent(QCloseEvent *event)
{
    for(int i=0;i<GroupWidgetList->size();i++){
        if(GroupWidgetList->at(i).name == groupName){
            GroupWidgetList->removeAt(i);
            break;
        }
    }
    event -> accept();
}



Group_chat::~Group_chat()
{
    delete ui;
}

