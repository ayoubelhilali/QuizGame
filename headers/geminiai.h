#ifndef GEMINIAI_H
#define GEMINIAI_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>


#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
class GeminiAI : public QObject {
    Q_OBJECT

public:
    explicit GeminiAI(QObject *parent = nullptr);
    void askQuestion(const QString &question, QString domain);
    void parseAndStoreQuestions(const QByteArray& response,QString domain);
    void processTextResponse(const QByteArray &responseData);
    void setApiKey(const QString &key);
    void processJsonQuestionsResponse(const QByteArray &responseData, const QString &domain);
    QVector<QJsonObject> getQuestionsFromDB();
    QString extractJsonTextFromGemini(const QByteArray &responseData);
    QString cleanJsonText(const QString &rawText);
signals:
    void responseReceived(const QString &response);



private:
    QNetworkAccessManager networkManager;
    QString apiKey;
};

#endif // GEMINIAI_H
