#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    socket = new QTcpSocket;
    socket->connectToHost(QHostAddress("8.208.11.15"), 8000);
    //连接信号和槽
    connect(socket,&QTcpSocket::connected, this, &Widget::connect_success); //服务器连接成功
    connect(socket,&QTcpSocket::readyRead, this, &Widget::server_reply);    //服务器回复

}

Widget::~Widget()
{
    delete ui;
}

void Widget::connect_success()
{
    //QMessageBox::information(this, "connection_status","connection_success");
}


void Widget::server_reply()
{
    //读取socket中的数据
    QByteArray ba = socket->readAll();
    QJsonObject obj = QJsonDocument::fromJson(ba).object();
    QString cmd = obj.value("cmd").toString();
    qDebug()<<obj.value("result").toString();
    if(cmd == "register_reply"){
        client_geister_handler(obj.value("result").toString()); //判断注册结果
    }
    else if(cmd == "login_reply"){
        client_login_handler(obj.value("result").toString(), obj.value("friend").toString(),obj.value("group").toString());
    }
}


//注册
void Widget::on_Register_PB_clicked()
{
    //获取文本
    QString username=ui->User_lineEdit->text();
    QString password=ui->password_lineEdit->text();

    //创建注册的json对象
    QJsonObject obj;
    obj.insert("cmd","register");
    obj.insert("user",username);
    obj.insert("password",password);

    //发送json对象
    QByteArray ba = QJsonDocument(obj).toJson(); //将json对象转换为ByteArray对象
    socket-> write(ba);
}
void Widget::client_geister_handler(QString result)
{
    if(result == "success"){
        QMessageBox::information(this, "注册结果", "注册成功");
    }
    else if(result == "failure"){
        QMessageBox::information(this, "注册结果", "注册失败");
    }
}


//登陆
void Widget::on_LogIn_PB_clicked()
{
    //获取文本
    QString username=ui->User_lineEdit->text();
    QString password=ui->password_lineEdit->text();

    //创建注册的json对象
    QJsonObject obj;
    obj.insert("cmd","login");
    obj.insert("user",username);
    obj.insert("password",password);

    userName = username;
    //发送json对象
    QByteArray ba = QJsonDocument(obj).toJson(); //将json对象转换为ByteArray对象
    socket-> write(ba);
}
void Widget::client_login_handler(QString result,QString fri, QString group)
{
    if(result == "user_not_exist"){
        QMessageBox::warning(this, "登陆提示","用户不存在");
    }
    else if(result == "password_error"){
        QMessageBox::warning(this, "登陆提示","密码错误");
    }
    else if(result == "success"){
        this->hide(); //socket会断开
        socket->disconnect(SIGNAL(readyRead()));
        Chatlist *c = new Chatlist(socket, fri, group, userName);
        c->setWindowTitle(userName);
        c->show();
    }
}


