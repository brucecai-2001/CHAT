#ifndef CREATEGROUP_H
#define CREATEGROUP_H

#include <QWidget>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>

namespace Ui {
class CreateGroup;
}

class CreateGroup : public QWidget
{
    Q_OBJECT

public:
    explicit CreateGroup(QTcpSocket *s, QString u, QWidget *parent = nullptr);
    ~CreateGroup();

private slots:
    void on_CancelPb_clicked();
    void on_CreatePb_clicked();

private:
    Ui::CreateGroup *ui;
    QString userName;
    QTcpSocket *socket;
};

#endif // CREATEGROUP_H
