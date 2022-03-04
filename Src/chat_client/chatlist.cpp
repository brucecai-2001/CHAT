#include "chatlist.h"
#include "ui_chatlist.h"

Chatlist::Chatlist(QTcpSocket *s, QString fri, QString group, QString u, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Chatlist)
{
    ui->setupUi(this);

    userName=u;
    socket=s;

    connect(socket,&QTcpSocket::readyRead, this, &Chatlist::server_reply); //服务器消息
    connect(ui->FriendList,&QListWidget::itemDoubleClicked, this, &Chatlist::FriendList_double_click); //双击好友
    connect(ui->GroupList,&QListWidget::itemDoubleClicked, this, &Chatlist::GroupList_double_click); //双击群聊
    //解析好友列表
    QStringList friend_list= fri.split('|');
    for(int i=0;i<friend_list.size();i++){  //将字符串组中的好友添加到屏幕上
        if(friend_list.at(i) != ""){
            ui->FriendList->addItem(friend_list.at(i));
        }
    }
    //解析群聊列表
    QStringList group_list= group.split('|');
    for(int i=0;i<group_list.size();i++){  //将字符串组中的群添加到屏幕上
        if(group_list.at(i) !="") ui->GroupList->addItem(group_list.at(i));
    }
}

//服务器回复
void Chatlist::server_reply()
{
    qDebug()<<"receive reply";
    QByteArray ba = socket->readAll();
    QJsonObject obj = QJsonDocument::fromJson(ba).object();
    QString cmd = obj.value("cmd").toString();
    if(cmd == "friend_login"){  //好友上线提醒
        client_login_reply(obj.value("friend").toString());
    }
    else if(cmd == "add_reply"){   //主动添加好友
        client_add_friend(obj.value("result").toString(),obj.value("friend").toString());
    }
    else if(cmd == "add_friend_reply"){   //被添加好友
        QString str = QString("%1把你添加为好友").arg(obj.value("result").toString());
        QMessageBox::information(this,"好友添加提醒",str);
        ui->FriendList->addItem(obj.value("result").toString());
    }
    else if(cmd == "create_group_reply"){  //创建群里
        client_create_group_reply(obj.value("result").toString(),obj.value("group").toString());
    }
    else if(cmd == "add_group_reply"){    //添加群聊
        client_add_group_reply(obj.value("result").toString(), obj.value("group").toString());
    }
    else if(cmd == "private_chat_reply"){ //我方发送信息
        client_private_chat_reply(obj.value("result").toString());
    }
    else if(cmd == "private_chat"){  //对方发送信息
        client_receive_chat(obj);
    }
    else if(cmd == "get_group_member_reply"){
        client_get_GroupMember_reply(obj); //收到群成员
    }
    else if(cmd == "group_chat"){  //收到群聊
        client_group_chat_reply(obj);
    }
    else if(cmd == "send_file_reply"){  //发送文件回复
        client_send_file_reply(obj.value("result").toString());
    }
    else if(cmd == "send_file_port_reply"){ //回复端口号
        client_send_file_port_reply(obj);
    }
    else if(cmd == "recv_file_port_reply"){
        client_recv_file_port_reply(obj);
    }
    else if(cmd == "friend_offline"){
        client_friend_offline(obj.value("friend").toString());
    }
}

//好友上线提醒
void Chatlist::client_login_reply(QString fri)
{
    QString str = QString("%1好友上线").arg(fri);
    QMessageBox::information(this,"好友上线提醒",str);
}


//添加好友
void Chatlist::on_AddPb_clicked()
{
    //打开一个添加好友的界面
    AddFriend *addFriendWidget = new AddFriend(socket,userName);
    addFriendWidget->show();
}

void Chatlist::client_add_friend(QString res, QString fn)
{
    if(res== "user_not_exist"){
        QMessageBox::warning(this,"添加好友提醒","好友不存在");
    }
    else if(res == "already_friend"){
        QMessageBox::warning(this,"添加好友提醒","已经是好友了");
    }
    else if(res== "success"){
        QMessageBox::information(this,"添加好友提醒","添加成功");
        ui->FriendList->addItem(fn); // 打印好友
    }
}


//创建群聊
void Chatlist::on_CreatePb_clicked()
{
    CreateGroup *creategroupWidget = new CreateGroup(socket, userName);
    creategroupWidget->show();
}

void Chatlist::client_create_group_reply(QString res, QString groupName)
{
    if(res == "group_exist"){
        QMessageBox::warning(this, "创建群提示", "群已经存在");
    }
    else if(res == "success"){
        ui->GroupList->addItem(groupName);
    }
}



//添加群聊
void Chatlist::on_AddGroupPb_clicked()
{
    AddGroup *addgroupWidget = new AddGroup(socket, userName);
    addgroupWidget->show();
}


void Chatlist::client_add_group_reply(QString res, QString groupName)
{
    if(res == "group_not_exist"){
        QMessageBox::warning(this,"添加群提示","群不存在");
    }
    else if(res == "user_in_group"){
        QMessageBox::warning(this,"添加群提示","你已经在群里了");
    }
    else if(res == "success"){
        ui->GroupList->addItem(groupName);
    }
}




//私聊
void Chatlist::FriendList_double_click()  //双击好友
{
    QString friend_name=ui->FriendList->currentItem()->text();
    Private_Chat *pcWidget = new Private_Chat(socket,userName,friend_name,this,&widgetList);
    pcWidget->setWindowTitle(friend_name);
    pcWidget->show();

    ChatInfo c= {pcWidget, friend_name}; //当前窗口加入链表
    widgetList.push_back(c);
}

void Chatlist::client_private_chat_reply(QString res) //发送回复
{
    if(res == "offline"){
        QMessageBox::warning(this,"发送提醒","对方不在线");
    }
}

void Chatlist::client_receive_chat(QJsonObject &obj) //收到消息
{
    //判断窗口是否已经打开
    bool flag=false;
    for(int i=0;i<widgetList.size();i++){
        if(widgetList.at(i).name == obj.value("user_from").toString()){
            flag=true;
            break;
        }
    }
    if(!flag){//创建一个新窗口
        Private_Chat *pcWidget = new Private_Chat(socket,userName,obj.value("user_from").toString(),this, &widgetList);
        pcWidget->setWindowTitle(obj.value("user_from").toString());
        pcWidget->show();
        ChatInfo c= {pcWidget, obj.value("user_from").toString()}; //当前窗口加入链表
        widgetList.push_back(c);
    }
    emit signal_to_Private_widget(obj); //给pcWidget 发送信号
}




//群聊
void Chatlist::GroupList_double_click()
{
    QString groupName = ui->GroupList->currentItem()->text();
    Group_chat *gc = new Group_chat(socket,userName,groupName,this,&GroupWidgetList);
    gc->setWindowTitle(groupName);
    gc->show();
    GroupInfo g  = {gc, groupName};
    GroupWidgetList.push_back(g);
}

void Chatlist::client_get_GroupMember_reply(QJsonObject obj)
{
    emit signal_to_Group_widget(obj);
}

void Chatlist::client_group_chat_reply(QJsonObject obj)
{
    bool flag=0;
    for(int i=0;i<GroupWidgetList.size();i++){
        if(GroupWidgetList.at(i).name == obj.value("group").toString()){
            flag=1;
            break;
        }
    }
    if(!flag){
        QString groupName = obj.value("group").toString();
        Group_chat *gc = new Group_chat(socket,userName,groupName,this, &GroupWidgetList);
        gc->setWindowTitle(groupName);
        gc->show();
        GroupInfo g  = {gc, groupName};
        GroupWidgetList.push_back(g);
    }
    emit signal_to_Group_Widget_chat(obj);
}



//文件
void Chatlist::client_send_file_reply(QString res)
{
    if(res == "offline"){  //对方不在线
        QMessageBox::warning(this,"发送提醒","对方不在线");
    }
    else if(res == "timeout"){
        QMessageBox::warning(this,"发送提醒","连接超时");
    }

}

void Chatlist::client_send_file_port_reply(QJsonObject obj)
{
    SendThread *sendThread=new SendThread(obj); //发送线程
    sendThread->start();
}

void Chatlist::client_recv_file_port_reply(QJsonObject obj)
{
    RecvThread *recvThread = new RecvThread(obj);
    recvThread->start();
}

void Chatlist::client_friend_offline(QString fri)
{
    QString str= QString("%1下线了").arg(fri);
    QMessageBox::warning(this,"好友下线",str);
}




//用户下线
void Chatlist::closeEvent(QCloseEvent *event)
{
    QJsonObject obj;
    obj.insert("cmd","offline");
    obj.insert("user",userName);

    QByteArray ba = QJsonDocument(obj).toJson();
    socket->write(ba);
    socket->flush();

    event->accept();
}

Chatlist::~Chatlist()
{
    delete ui;
}



