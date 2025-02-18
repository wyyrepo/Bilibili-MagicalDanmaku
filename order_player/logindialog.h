#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "songbeans.h"
#include "netinterface.h"

namespace Ui {
class LoginDialog;
}

#define NETEASE_SERVER QString("http://iwxyi.com:3000")
#define QQMUSIC_SERVER QString("http://iwxyi.com:3300")
#define MIGU_SERVER QString("http://iwxyi.com:3400")
#define KUGOU_SERVER QString("") // 这个暂时不固定

class LoginDialog : public QDialog, public NetInterface
{
    Q_OBJECT
public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog() override;

private slots:
    void on_loginButton_clicked();

    void on_neteaseRadio_clicked();

    void on_cookieHelpButton_clicked();

    void on_neteaseCookieRadio_clicked();

    void on_qqmusicCookieRadio_clicked();

    void on_testButton_clicked();

private:
    void loginNetease(QString username, QString password);
    void cookieNetease(QString cookies);
    void cookieQQMusic(QString cookies);

signals:
    void signalLogined(MusicSource source, QString cookies);

private:
    Ui::LoginDialog *ui;
};

#endif // LOGINDIALOG_H
