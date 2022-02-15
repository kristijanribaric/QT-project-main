#include "clientudp.h"
#include "ui_clientudp.h"
#include "cipher.h"
#include <QUdpSocket>

QByteArray getPrivateKey() {
    QByteArray testPrivateKey;
    testPrivateKey.append("-----BEGIN RSA PRIVATE KEY-----\n");
    testPrivateKey.append("MIIEpAIBAAKCAQEAntGSOUusaJACRVWYWIp+AqlUd155lKK56fQBg0es0nyYNi+p\n");
    testPrivateKey.append("s3j+k9qJh9hmqQ9FyugugPe1rgvjMbyLrI/PXGUcpyO98bIrJwNLr9s0apFt6YfB\n");
    testPrivateKey.append("SDJVUnHhvbXGBBoCyxUiMnc1hUDs/JDkEJI7DcfTLxO4cERIVVtJQw+JsUuLS/sJ\n");
    testPrivateKey.append("9BrBKM94FDYrVxCqDL1znq7OCiIeWPoT4Nvw9tPoFortncDFCPqmaj7eppXFKq4o\n");
    testPrivateKey.append("KKpDeuwZYAS+M23qw8x+DtfXSaGutZOJSK7vAWeeMac7e9dEs8y7Hez31DyWOHkY\n");
    testPrivateKey.append("+0ymZ134Zl8MSc7MdbGWh0O8tq2F9P3ogSf7ZwIDAQABAoIBADO6iugCj9tW3IZ8\n");
    testPrivateKey.append("xKxCH5z6Tjq7GNqize83vK5n0BCW6HTtvVZpmHHMAnOutnFmOhNG4K68//36fTub\n");
    testPrivateKey.append("79p61M2BWX3PFHT+1TNVzXcv7LixOZ2QzBGZ41SBJ0fFRwPa/QHslkDdtx758Laq\n");
    testPrivateKey.append("8r27A2UPqudwguh+NRkoLIqdKvoF35S4lPVwZN4+hqu3AGr9/ea3h+wm9k4usvpu\n");
    testPrivateKey.append("B9IoZIVZvQX3zo8t0KQNFTFE+qEn7Ps1djvCmsAWdJl53V8zgMqmjKMdsZ8St118\n");
    testPrivateKey.append("k7nahpUjAsxeiKPZ4wkOkh43UElVVbkZ/H9qCOOrNd6nB7rJsTlnH4ELo+/aNGsu\n");
    testPrivateKey.append("vLoPEsECgYEA0qhT8F9BL05c0lWR8sl/KPK3VC1kyhQ+c2c6y5De+/P0ErYt3rCI\n");
    testPrivateKey.append("35OjgprRXESfn31ZiXOTTMhQ1L8b3JrK/SFVPuq6vV/itzrU6x5zHR+NI889YgrP\n");
    testPrivateKey.append("T9iftL7naX2TiUUcRsuwKhwKOq+WIxtb9dNJ1sJR+tFZM8MYphxrv/cCgYEAwQDP\n");
    testPrivateKey.append("vFMWtKxIU1nJCZbGJG+o7jQdZX8yHHq3v3iwe3p8+9RAs2i9kjSVCD8UVNsnpvJb\n");
    testPrivateKey.append("MuByJsHARRXJZtkr71wZeIsCNE/r1ryJyWHUY1X/UirxQshI8n6GC8Ivhmpbcale\n");
    testPrivateKey.append("uIEh6icPOCOSy16MBfgaKxRfGMEb84fMdYuqpBECgYEAmhn1PZQRFVAX6u4aFCd+\n");
    testPrivateKey.append("1C+qGPQUFRf2D2nmwt+5n0p4s+99Yrh1iEhyhqpGmV7B/mQuSbS4fqTTLK6Y7Sp3\n");
    testPrivateKey.append("b/EgcSQ5cytfqHZrJ4xg3V8lWprR5pTNzzVCr3AvzSXF9dcOtmNu/X8BpKiqBgiP\n");
    testPrivateKey.append("nybvjyEb44Wd4ZXIxEtqL0UCgYBZ6wvdfIR2llF367u66PDVZuQ4XjVX5SOumwni\n");
    testPrivateKey.append("fdMTJCT4mpGuRF0VscPBPMo3Ioddw0nz8RedsOToCtdzQDi04p7HR1Q2lQmWWO6E\n");
    testPrivateKey.append("0TxHL3itim4uo+tAduvyrHzBzelekf8cTKVFVv9DM2efsiPgYA+pBGutt8JS+nCq\n");
    testPrivateKey.append("TvCoUQKBgQDSnRl8U1zT5DgAuOtgO23oato2PBycRjN3dsMXKr+2eS+jdFucRw32\n");
    testPrivateKey.append("Rly6wCduuhwy4AcrCpcxmekuiYKuR2UTpdvDyQuae7a8O8P0XPF08hxpG55GmOfE\n");
    testPrivateKey.append("FyjL7AohBuU/coPXzQ6hhlIzMFqWHtc6L/4oWJxItwF27vjn4w1Dqw==\n");
    testPrivateKey.append("-----END RSA PRIVATE KEY-----\n");


    return testPrivateKey;
}

QByteArray decryptRSA (QByteArray encrypted) {
    QByteArray testPrivateKey = getPrivateKey();

    Cipher cWrapper;
    RSA* privatekey = cWrapper.getPrivateKey(testPrivateKey);

    QByteArray decrypted = cWrapper.decryptRSA(privatekey,encrypted);
    cWrapper.freeRSAKey(privatekey);
    return decrypted;
}

ClientUDP::ClientUDP(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClientUDP)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("UDP Generator"));
    socket = new QUdpSocket(this);
    connect(socket,&QUdpSocket::readyRead, [&](){
        if(socket->hasPendingDatagrams()){
            QByteArray datagram;
            datagram.resize(socket->pendingDatagramSize());
            socket->readDatagram(datagram.data(),datagram.size());
            QByteArray list = decryptRSA(datagram);
            ui->listWidget->clear();
            ui->listWidget->addItem(list);

        }

    });
}

ClientUDP::~ClientUDP()
{
    delete ui;
}



void ClientUDP::on_connectButton_clicked()
{
    socket->bind(ui->portBox->value(),QUdpSocket::ShareAddress);
}

