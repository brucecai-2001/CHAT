#include "private_chat.h"
#include "ui_private_chat.h"

Private_Chat::Private_Chat(QTcpSocket *s,QString u, QString f,Chatlist *c, QList<ChatInfo> *wl,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Private_Chat)
{
    ui->setupUi(this);
    socket = s;
    userName = u;
    friendName = f;
    mainWidget = c;
    widgetList = wl;
    connect(mainWidget, &Chatlist::signal_to_Private_widget, this, &Private_Chat::receive_text); //连接主窗口的信号
}

Private_Chat::~Private_Chat()
{
    delete ui;
}



void Private_Chat::on_SendPb_clicked() //发送消息
{
    QString text = ui->lineEdit->text();
    QJsonObject obj;
    obj.insert("cmd","private_chat");
    obj.insert("user_from",userName);
    obj.insert("user_to",friendName);
    obj.insert("text",text); //打包消息

    QByteArray ba = QJsonDocument(obj).toJson();
    socket->write(ba);//发送消息

    ui->lineEdit->clear(); //清空输入
    ui->textEdit->append(text);
    ui->textEdit->append("\n");
}

void Private_Chat::receive_text(QJsonObject obj) //接受信息
{
    if(obj.value("cmd").toString() == "private_chat"){//判断是不是发给自己的
        if(obj.value("user_from").toString() == friendName){
            if(this->isMinimized()) this->showNormal();
            this->activateWindow();
            ui->textEdit->append(obj.value("text").toString()); //显示信息
            ui->textEdit->append("\n");
        }
    }
}


//关闭窗口
void Private_Chat::closeEvent(QCloseEvent *event)
{
    for(int i=0;i<widgetList->size();i++){
        if(widgetList->at(i).name == friendName){
            widgetList->removeAt(i);
            break;
        }
    }
    event -> accept();
}

//发送文件
void Private_Chat::on_FilePb_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,"选择文件",QCoreApplication::applicationFilePath());
    if(fileName.isEmpty()){
        QMessageBox::warning(this,"发送文件提示","选择一个文件");
    }
    else{
        QFile file(fileName);
        file.open(QIODevice::ReadOnly); //打开文件
        QJsonObject obj;                //创建Json对象
        obj.insert("cmd","send_file");
        obj.insert("from_user",userName);
        obj.insert("to_user",friendName);
        obj.insert("length",file.size());
        obj.insert("filename",fileName);

        QByteArray ba = QJsonDocument(obj).toJson();
        socket->write(ba);
    }
}

