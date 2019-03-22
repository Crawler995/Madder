#ifndef IMAGEDOWNLOADER_H
#define IMAGEDOWNLOADER_H

#include <QtCore>
#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QByteArray>
#include <QPixmap>
#include <QFile>

class ImageDownloader: public QObject
{
    Q_OBJECT
public:
    ImageDownloader(QString urlString) {
        this->urlString = urlString;
    }

    QNetworkReply* beginDownload() {
        QUrl url(urlString);
        manager = new QNetworkAccessManager(this);
        QNetworkRequest request;

        request.setUrl(url);

        connect(manager, SIGNAL(finished(QNetworkReply*)), this,
                SLOT(downloadFinished(QNetworkReply*)));

        downloadReply = manager->get(request);

        return downloadReply;
    }

    QNetworkAccessManager *manager;
    QNetworkReply *downloadReply;
    QString urlString;

private slots:
    void downloadFinished(QNetworkReply* reply) {
        if(reply->error() == QNetworkReply::NoError) {
           QByteArray bytes = reply->readAll();

           QFile file("download.jpg");
           if (file.open(QIODevice::WriteOnly))
           {
               file.write(bytes);
           }
           file.close();

           emit downloadFinishedAndSaved();
        }
    }

signals:
    void downloadFinishedAndSaved();
};

#endif // IMAGEDOWNLOADER_H
