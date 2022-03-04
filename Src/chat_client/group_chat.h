#ifndef GROUP_CHAT_H
#define GROUP_CHAT_H

#include <QWidget>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QList>
#include <QCloseEvent>
class Chatlist;
struct GroupInfo;
#include "chatlist.h"

namespace Ui {
class Group_chat;
}

class Group_chat : public QWidget
{
    Q_OBJECT

public:
    explicit Group_chat(QTcpSocket *s, QString u, QString g ,Chatlist *c ,QList<GroupInfo> *gl, QWidget *parent = nullptr);
    ~Group_chat();

    void closeEvent(QCloseEvent *event);//关闭窗口

private slots:
    void Group_member(QJsonObject);
    void show_chat_text(QJsonObject);
    void on_SendPb_clicked();

private:
    Ui::Group_chat *ui;
    QTcpSocket *socket;
    QString userName;
    QString groupName;
    Chatlist *mainWidget;
    QList<GroupInfo> *GroupWidgetList;
};

#endif // GROUP_CHAT_H
