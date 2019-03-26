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
#include <QVariant>
#include <QDebug>

/**
 * @brief The ImageDownloader class
 *
 * The class provides some operations of download images.
 *
 * If the class is transferred, it will be moved in another thread which is different from the main thread,
 * in order that avoid to block the progress dialog. If the image downloader runs in main thread,
 * there will be serval seconds block that after the progress dialog appears and before the progress bar appears.
 * (According to the console, I think the serval seconds may be used to initialize the network service.)
 */
class ImageDownloader: public QObject
{
    Q_OBJECT
public:
    /**
     * @brief ImageDownloader
     * @param parent the mainWindow.
     *
     * The paramter "parent" is used to connect the signal (in this class) and slot (in main window).
     * Because they cross between two threads, according to much debug I find that maybe the connect
     * operation can only be done in this class.
     */
    ImageDownloader(QMainWindow *parent) {
        this->parent = parent;
    }

    QNetworkAccessManager *manager;
    QNetworkReply *downloadReply;
    QString urlString;
    QMainWindow *parent;

public slots:
    /**
     * @brief beginDownload
     * @param urlString the image url string.
     *
     * The slot is transferred by a signal in main thread.
     */
    void beginDownload(QString urlString) {
        this->urlString = urlString;
        manager = new QNetworkAccessManager();
        QUrl url(urlString);
        QNetworkRequest request;
        request.setUrl(url);

        connect(manager, SIGNAL(finished(QNetworkReply*)), this,
                SLOT(downloadFinished(QNetworkReply*)));

        downloadReply = manager->get(request);

        connect(downloadReply, SIGNAL(downloadProgress(qint64,qint64)), parent,
                SLOT(updateDownloadProgress(qint64,qint64)), Qt::QueuedConnection);
    }

private:
    /**
     * @brief getFileName
     * @param reply
     * @return the file name string.
     *
     * According to the download reply, return the file name.
     */
    QString getFileName(QNetworkReply* reply) {
        QString contentDisposition = reply->rawHeader("Content-Disposition");
        int index = contentDisposition.indexOf("filename=");
        QString fileName = contentDisposition.mid(index + 9);

        if(fileName.isEmpty()) {
            QUrl url(urlString);
            QFileInfo fileInfo(url.path());
            fileName = fileInfo.fileName();
        }

        return fileName;
    }

private slots:
    /**
     * @brief downloadFinished
     * @param reply
     *
     * If the image download finished successfully or appeared error, the slots will be triggered.
     * If download successfully, send the "downloadFinishedAndSavedSignal" signal to the main thread.
     * If there is a error, send the "downloadImageFailedSignal" signal to the main thread to open
     * a messagebox to show that download failed.
     */
    void downloadFinished(QNetworkReply* reply) {
        if(reply->error() == QNetworkReply::NoError) {
            QByteArray bytes = reply->readAll();
            QString fileName = getFileName(reply);
            QFile file(fileName);

            if (file.open(QIODevice::WriteOnly)) {
               file.write(bytes);
            }
            file.close();

            emit downloadFinishedAndSavedSignal(fileName);
        }
        else {
            emit downloadImageFailedSignal();
        }
    }

signals:
    void downloadFinishedAndSavedSignal(QString fileName);
    void downloadImageFailedSignal();
};

#endif // IMAGEDOWNLOADER_H
