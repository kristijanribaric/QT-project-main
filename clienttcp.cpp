#include "clienttcp.h"
#include "ui_clienttcp.h"
#include "cipher.h"
#include <QtWidgets>
#include <QtNetwork>

QByteArray DecryptAES (QByteArray encrypted) {

    Cipher cWrapper;
    QString passphrase = "password";
    QByteArray decrypted = cWrapper.decryptAES(passphrase.toLatin1(),encrypted);
    qDebug() << encrypted.toBase64();
    qDebug() << decrypted;
    return decrypted;

}


ClientTCP::ClientTCP(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClientTCP),
    tcpSocket(new QTcpSocket(this))

{
    ui->setupUi(this);
    this->setWindowTitle(tr("TCP Generator"));
    ui->hostCombo->setEditable(true);

    // IP adrese
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // adrese koje nisu od localhosta
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (!ipAddressesList.at(i).isLoopback())
             ui->hostCombo->addItem(ipAddressesList.at(i).toString());
    }
    // localhost adrese
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i).isLoopback())
             ui->hostCombo->addItem(ipAddressesList.at(i).toString());
    }

    ui->portLineEdit->setValidator(new QIntValidator(1, 65535, this));

    ui->getEmployeeButton->setDefault(true);
    ui->getEmployeeButton->setEnabled(false);

    in.setDevice(tcpSocket);
    in.setVersion(QDataStream::Qt_4_0);

    connect(ui->hostCombo, &QComboBox::editTextChanged,
            this, &ClientTCP::enableGetEmployeeButton);
    connect(ui->portLineEdit, &QLineEdit::textChanged,
            this, &ClientTCP::enableGetEmployeeButton);
    connect(ui->getEmployeeButton, &QAbstractButton::clicked,
            this, &ClientTCP::requestNewEmployee);
    connect(ui->quitButton, &QAbstractButton::clicked, this, &QWidget::close);
    connect(tcpSocket, &QIODevice::readyRead, this, &ClientTCP::readEmployee);
    connect(tcpSocket, &QAbstractSocket::errorOccurred,this, &ClientTCP::displayError);
}

void ClientTCP::requestNewEmployee()
{
    ui->getEmployeeButton->setEnabled(false);
    tcpSocket->abort();
    tcpSocket->connectToHost(ui->hostCombo->currentText(),
                             ui->portLineEdit->text().toInt());
}

void ClientTCP::readEmployee()
{
    in.startTransaction();

    QByteArray nextEmployee;
    in >> nextEmployee;

    if (!in.commitTransaction())
        return;

    if (nextEmployee == currentEmployee) {
        QTimer::singleShot(0, this, &ClientTCP::requestNewEmployee);
        return;
    }

    currentEmployee = nextEmployee;
    QByteArray final = DecryptAES(currentEmployee);
    ui->listWidget->clear();
    ui->listWidget->addItem(final);
    ui->getEmployeeButton->setEnabled(true);
}

void ClientTCP::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(this, tr("TCP Client"),
                                 tr("Host nije pronađen. Molim provjerite "
                                    "ime host-a i port."));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(this, tr("TCP Client"),
                                 tr("Konekcija je odbijena od strane peer-a. "
                                    "Provjerite da li je TCP server pokrenut "
                                    "te provjerite da li su postavke imena  "
                                    "hosta i port točni."));
        break;
    default:
        QMessageBox::information(this, tr("TCP Client"),
                                 tr("Dogodila se sljedeća greška: %1.")
                                 .arg(tcpSocket->errorString()));
    }

    ui->getEmployeeButton->setEnabled(true);
}

void ClientTCP::enableGetEmployeeButton()
{
    ui->getEmployeeButton->setEnabled(!ui->hostCombo->currentText().isEmpty() &&
                                 !ui->portLineEdit->text().isEmpty());

}

ClientTCP::~ClientTCP()
{
    delete ui;
}
