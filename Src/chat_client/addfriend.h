#ifndef ADDFRIEND_H
#define ADDFRIEND_H

#include <QWidget>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>

namespace Ui {
class AddFriend;
}

class AddFriend : public QWidget
{
    Q_OBJECT

public:
    explicit AddFriend(QTcpSocket *s, QString u,QWidget *parent = nullptr);
    ~AddFriend();

private slots:
    void on_CancelPb_clicked();
    void on_AddPb_clicked();

private:
    Ui::AddFriend *ui;
    QTcpSocket *socket;
    QString userName;
};

#endif // ADDFRIEND_H
