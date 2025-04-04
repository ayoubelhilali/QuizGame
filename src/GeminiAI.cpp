#include "headers/geminiai.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkRequest>
#include <QDebug>

GeminiAI::GeminiAI(QObject *parent) : QObject(parent) {
    apiKey = "AIzaSyD4MYY-Rko_93YRTeVFtuMOVF5KARm_q-0";  // Replace with your actual API key
    connect(&networkManager, &QNetworkAccessManager::finished, this, &GeminiAI::handleNetworkReply);
}

void GeminiAI::askQuestion(const QString &question) {
    QUrl url("https://generativelanguage.googleapis.com/v1/models/gemini-pro:generateText?key=" + apiKey);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject requestBody;
    requestBody["prompt"] = question;
    requestBody["max_tokens"] = 50;  // Limit response length

    QJsonDocument jsonDoc(requestBody);
    QByteArray requestData = jsonDoc.toJson();

    networkManager.post(request, requestData);
}

void GeminiAI::handleNetworkReply(QNetworkReply *reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
        QJsonObject jsonObject = jsonResponse.object();

        if (jsonObject.contains("candidates")) {
            QJsonArray candidates = jsonObject["candidates"].toArray();
            if (!candidates.isEmpty()) {
                QString aiResponse = candidates[0].toObject()["output"].toString();
                emit responseReceived(aiResponse);
                return;
            }
        }
    } else {
        qWarning() << "Network error:" << reply->errorString();
    }
    emit responseReceived("Error getting response from Gemini.");
    reply->deleteLater();
}
