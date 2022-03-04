#ifndef CHATLIST_H
#define CHATLIST_H

#include <QWidget>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>
#include <QCloseEvent>
#include <QList>
#include "addfriend.h" //添加好友窗口
#include "creategroup.h"  //创建群聊窗口
#include "addgroup.h"  //添加群窗口
class Private_Chat;
class Group_chat;
#include "private_chat.h" //私聊窗口
#include "group_chat.h" //群聊窗口
#include "sendthread.h" //发送线程
#include "recvthread.h" //接收线程

namespace Ui {
class Chatlist;
}

struct ChatInfo{
    Private_Chat *pcw;
    QString name;
};
struct GroupInfo{
    Group_chat *gcw;
    QString name;
};


class Chatlist : public QWidget
{
    Q_OBJECT

public:
    explicit Chatlist(QTcpSocket *, QString, QString, QString, QWidget *parent = nullptr);
    void closeEvent(QCloseEvent *event);
    ~Chatlist();


signals:
     void signal_to_Private_widget(QJsonObject); //给子窗口/好友窗口发送一个信号
     void signal_to_Group_widget(QJsonObject); //给群聊窗口，群成员列表
     void signal_to_Group_Widget_chat(QJsonObject); //显示群聊


private slots:
    void server_reply();
    void on_AddPb_clicked();
    void on_CreatePb_clicked();
    void on_AddGroupPb_clicked();
    void FriendList_double_click();
    void GroupList_double_click();

private:
    void client_login_reply(QString); //登陆
    void client_add_friend(QString,QString); //添加好友
    void client_create_group_reply(QString,QString);   //建群
    void client_add_group_reply(QString, QString); //添加群
    void client_private_chat_reply(QString); // 私聊回复
    void client_receive_chat(QJsonObject&); //收到私聊
    void client_get_GroupMember_reply(QJsonObject); //收到群成员
    void client_group_chat_reply(QJsonObject); //群聊
    void client_send_file_reply(QString);     //发送文件回复
    void client_send_file_port_reply(QJsonObject); //开启发送文件线程
    void client_recv_file_port_reply(QJsonObject obj); //开启接受发送文件线程
    void client_friend_offline(QString); //朋友下线
    Ui::Chatlist *ui;
    QTcpSocket *socket;
    QString userName;
    QList<ChatInfo>  widgetList;
    QList<GroupInfo> GroupWidgetList;
};

#endif // CHATLIST_H
