#ifndef CLIENTTCP_H
#define CLIENTTCP_H

#include <QDataStream>
#include <QDialog>
#include <QTcpSocket>
class QTcpSocket;
namespace Ui {
class ClientTCP;
}

class ClientTCP : public QDialog
{
    Q_OBJECT

public:
    explicit ClientTCP(QWidget *parent = nullptr);
    ~ClientTCP();

private slots:
    void requestNewEmployee();
    void readEmployee();
    void displayError(QAbstractSocket::SocketError socketError);
    void enableGetEmployeeButton();
private:
    Ui::ClientTCP *ui;

    QTcpSocket *tcpSocket = nullptr;
    QDataStream in;
    QByteArray currentEmployee;
};

#endif // CLIENTTCP_H
