#ifndef CLIENTUDP_H
#define CLIENTUDP_H

#include <QDialog>


namespace Ui {
class ClientUDP;
}
class QUdpSocket;
class ClientUDP : public QDialog
{
    Q_OBJECT

public:
    explicit ClientUDP(QWidget *parent = nullptr);
    ~ClientUDP();

private slots:
    void on_connectButton_clicked();

private:
    Ui::ClientUDP *ui;
    QUdpSocket *socket;
};

#endif // CLIENTUDP_H
