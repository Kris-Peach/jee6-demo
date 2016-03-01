#include "yantranslate.h"


YanTranslate::YanTranslate(QWidget *parent):QWidget(parent)
{
    QString key ("trnsl.1.1.20160223T163752Z.4ac0c18bf2406e04.f5b813437ccfb127de3d1bf07e0ad35d8474ec66");
    YanKey = key;
    QString url("https://translate.yandex.net/api/v1.5/tr.json/translate?");
    YanUrl = url;

}

void YanTranslate::setText(QString str)
{
    Text = str;
    return;
}

QString YanTranslate::getText()
{
    return Text;
}

void YanTranslate::setLang(QString str)
{
    Lang = str;
    return;
}

QString YanTranslate::getLang()
{
    return Lang;
}

QString YanTranslate::yantranslate()
{
    QUrl url(YanUrl);
    QUrlQuery current(url);
    current.addQueryItem("key", YanKey);
    current.addQueryItem("text", Text);
    current.addQueryItem("lang", Lang);
    url.setQuery(current);

    QByteArray answer = GET(url);
    qDebug() << answer;
    if (answer.isEmpty())
    {
        qDebug() <<"Пустой ответ от Яндекс.Переводчик";
        exit(-1);
    }
    QString translation (parsJson(answer));
    return translation;

}

QByteArray YanTranslate::GET(QUrl r)
{
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    QNetworkReply* reply = manager->get(QNetworkRequest (r));
    QEventLoop wait;
    connect(manager, SIGNAL(finished(QNetworkReply*)), &wait, SLOT(quit())); //когда придет ответ, мы выйдем из loop
    QTimer::singleShot(1000, &wait, SLOT(quit()));
    connect(manager, SIGNAL(finished(QNetworkReply*)), manager, SLOT(deleteLater()));
    wait.exec();

    QByteArray answer = reply->readAll();  //считываем ответ
    reply->deleteLater();
    return answer;
}

QString YanTranslate::parsJson(QByteArray answer)
{
    QString translation("");
    QJsonDocument jsondoc = QJsonDocument::fromJson(answer);
    int code = jsondoc.object()["code"].toInt();
    if (code == 200)
    {
        QJsonArray array = jsondoc.object()["text"].toArray();
        for(QJsonArray::Iterator iter = array.begin();iter!=array.end();  ++iter)
            {
            translation.append((*iter).toString());
        }
    }
    else {
       translation = parsError(code);
    }
    return translation;
}

QString YanTranslate::parsError(int code)
{
    QString ans("");
    switch (code) {
    case 401:
       { ans = "Неправильный ключ API";}
        break;
    case 402:
        {ans = "Ключ API заблокирован";}
        break;
    case 403:
        {ans = "Превышено суточное ограничение на количество запросов";}
        break;
    case 404:
        {ans = "Превышено суточное ограничение на объем переведенного текста";}
        break;
    case 413:
        {ans = "Превышен максимально допустимый размер текста";}
        break;
    case 422:
        {ans = "Текст не может быть переведен";}
        break;
    case 501:
        {ans = "Заданное направление перевода не поддерживается";}
        break;
    default:
        break;
    }
    return ans;
}


