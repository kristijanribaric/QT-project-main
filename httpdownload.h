#ifndef HTTPDOWNLOAD_H
#define HTTPDOWNLOAD_H

#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QProgressDialog>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QMessageBox>


namespace Ui {
class HttpDownload;
}

class HttpDownload : public QDialog
{
    Q_OBJECT

public:
    explicit HttpDownload(QWidget *parent = 0);
    ~HttpDownload();

public:
    void startRequest(QUrl url);
private slots:
    void on_downloadButton_clicked();

    void on_quitButton_clicked();

    void on_urlEdit_returnPressed();

    void httpReadyRead();

    void httpDownloadFinished();

    void updateDownloadProgress(qint64, qint64);

    void enableDownloadButton();
    void cancelDownload();

private:
    Ui::HttpDownload *ui;
    QUrl url;
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QProgressDialog *progressDialog;
    QFile *file;
    bool httpRequestAborted;
    qint64 fileSize;

};
#endif // HTTPDOWNLOAD_H
