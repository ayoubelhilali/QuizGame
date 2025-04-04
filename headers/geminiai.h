#ifndef GEMINIAI_H
#define GEMINIAI_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class GeminiAI : public QObject {
    Q_OBJECT

public:
    explicit GeminiAI(QObject *parent = nullptr);
    void askQuestion(const QString &question);

signals:
    void responseReceived(const QString &response);

private slots:
    void handleNetworkReply(QNetworkReply *reply);

private:
    QNetworkAccessManager networkManager;
    QString apiKey;
};

#endif // GEMINIAI_H
