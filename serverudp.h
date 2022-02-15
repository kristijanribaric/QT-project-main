#ifndef SERVERUDP_H
#define SERVERUDP_H


#include <QObject>
#include <QUdpSocket>

class ServerUDP : public QObject
{
    Q_OBJECT
public:
    explicit ServerUDP(QObject *parent = nullptr);

signals:

public slots:
    void readyRead();
};

#endif // SERVERUDP_H
