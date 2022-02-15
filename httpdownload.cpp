#include "httpdownload.h"
#include "ui_httpdownload.h"

HttpDownload::HttpDownload(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HttpDownload)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("HTTP Preuzimanje"));
    ui->urlEdit->setText("http://projanco.com/Library/JSON%20at%20Work.pdf");
    ui->statusLabel->setWordWrap(true);
    ui->downloadButton->setDefault(true);
    ui->quitButton->setAutoDefault(false);


}

HttpDownload::~HttpDownload()
{
    delete ui;
}



void HttpDownload::httpReadyRead()
{
    if (file)
        file->write(reply->readAll());
}

void HttpDownload::updateDownloadProgress(qint64 bytesRead, qint64 totalBytes)
{
    if (httpRequestAborted)
        return;

    progressDialog->setMaximum(totalBytes);
    progressDialog->setValue(bytesRead);
}



void HttpDownload::on_urlEdit_returnPressed()
{
    on_downloadButton_clicked();
}

void HttpDownload::enableDownloadButton()
{
    ui->downloadButton->setEnabled(!(ui->urlEdit->text()).isEmpty());
}

void HttpDownload::cancelDownload()
{
    ui->statusLabel->setText(tr("Preuzimanje prekinuto."));
    httpRequestAborted = true;
    reply->abort();
    ui->downloadButton->setEnabled(true);
}

void HttpDownload::httpDownloadFinished()
{
    if (httpRequestAborted) {
        if (file) {
            file->close();
            file->remove();
            delete file;
            file = 0;
        }
        reply->deleteLater();
        progressDialog->hide();
        return;
    }

    progressDialog->hide();
    file->flush();
    file->close();

    QVariant redirectionTarget = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (reply->error()) {
        file->remove();
        QMessageBox::information(this, tr("HTTP"),
                                 tr("Preuzimanje nije uspješno: %1.")
                                 .arg(reply->errorString()));
        ui->downloadButton->setEnabled(true);
    } else if (!redirectionTarget.isNull()) {
        QUrl newUrl = url.resolved(redirectionTarget.toUrl());
        if (QMessageBox::question(this, tr("HTTP"),
                                  tr("Preusmjeriti na %1 ?").arg(newUrl.toString()),
                                  QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
            url = newUrl;
            reply->deleteLater();
            file->open(QIODevice::WriteOnly);
            file->resize(0);
            startRequest(url);
            return;
        }
    } else {
        QString fileName = QFileInfo(QUrl(ui->urlEdit->text()).path()).fileName();
        ui->statusLabel->setText(tr("Preuzeta datoteka %1 u direktorij %2.").arg(fileName).arg(QDir::currentPath()));
        ui->downloadButton->setEnabled(true);
    }

    reply->deleteLater();
    reply = 0;
    delete file;
    file = 0;
    manager = 0;
}


void HttpDownload::startRequest(QUrl url)
{

    reply = manager->get(QNetworkRequest(url));


    connect(reply, SIGNAL(readyRead()),
            this, SLOT(httpReadyRead()));


    connect(reply, SIGNAL(downloadProgress(qint64,qint64)),
            this, SLOT(updateDownloadProgress(qint64,qint64)));

    connect(reply, SIGNAL(finished()),
            this, SLOT(httpDownloadFinished()));
}

void HttpDownload::on_downloadButton_clicked()
{
    progressDialog = new QProgressDialog(this);
    progressDialog->setAutoClose(true);

    connect(ui->urlEdit, SIGNAL(textChanged(QString)),
                this, SLOT(enableDownloadButton()));
    connect(progressDialog, SIGNAL(canceled()), this, SLOT(cancelDownload()));

    manager = new QNetworkAccessManager(this);
    url = (ui->urlEdit->text());

    QFileInfo fileInfo(url.path());
    QString fileName = fileInfo.fileName();

    if (fileName.isEmpty())
        fileName = "index.html";

    if (QFile::exists(fileName)) {
        if (QMessageBox::question(this, tr("HTTP"),
                tr("Datoteka %1  "
                "već postoji u zadanom direktoriju. Prepisati?").arg(fileName),
                QMessageBox::Yes|QMessageBox::No, QMessageBox::No)
                == QMessageBox::No)
                return;
        QFile::remove(fileName);
    }

    file = new QFile(fileName);
    if (!file->open(QIODevice::WriteOnly)) {
        QMessageBox::information(this, tr("HTTP"),
                      tr("Unable to save the file %1: %2.")
                      .arg(fileName).arg(file->errorString()));
        delete file;
        file = 0;
        return;
    }


    httpRequestAborted = false;

    progressDialog->setWindowTitle(tr("HTTP"));
    progressDialog->setLabelText(tr("Preuzimanje %1.").arg(fileName));

    ui->downloadButton->setEnabled(false);

    startRequest(url);
}

void HttpDownload::on_quitButton_clicked()
{
    this->close();
}
