#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QHostAddress>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>
#include <chatlist.h>


QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void connect_success();
    void server_reply();
    void on_Register_PB_clicked();
    void on_LogIn_PB_clicked();

private:
    void client_geister_handler(QString);
    void client_login_handler(QString,QString,QString);
    Ui::Widget *ui;
    QTcpSocket *socket;
    QString userName; //当前用户的用户名
};
#endif // WIDGET_H
