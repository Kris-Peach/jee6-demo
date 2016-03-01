#ifndef YANTRANSLATE_H
#define YANTRANSLATE_H
#include <QWidget>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QTimer>
#include <QEventLoop>
#include <QUrlQuery>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <iterator>

class YanTranslate: public QWidget
{
    Q_OBJECT
private:
    QString Text;
    QString YanKey;
    QString Lang;
    QUrl YanUrl;
    QByteArray GET(QUrl);
    QString parsJson(QByteArray);
    QString parsError(int);
public:
    YanTranslate(QWidget *parent = 0);
    void setText(QString);
    QString getText();
    void setLang(QString);
    QString getLang();
    QString yantranslate();

};

#endif // YANTRANSLATE_H
