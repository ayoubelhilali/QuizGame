#include "headers/geminiai.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkRequest>
#include <QDebug>

GeminiAI::GeminiAI(QObject *parent) : QObject(parent) {
    apiKey = "AIzaSyD5VEqZ_JGri9TP4OFzDI0rarUgpYWy_U0"; // Remplace par ta vraie clé API
    connect(&networkManager, &QNetworkAccessManager::finished, this, &GeminiAI::handleNetworkReply);
}

void GeminiAI::askQuestion(const QString &question) {
    // 🔗 URL de l'API Gemini
    QUrl url(QString("https://generativelanguage.googleapis.com/v1beta/models/gemini-2.0-flash:generateContent?key=%1").arg(apiKey));
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // 📦 Construction du corps JSON
    QJsonObject textPart;
    textPart["text"] = question;

    QJsonArray partsArray;
    partsArray.append(textPart);

    QJsonObject contentItem;
    contentItem["parts"] = partsArray;

    QJsonArray contentsArray;
    contentsArray.append(contentItem);

    QJsonObject requestBody;
    requestBody["contents"] = contentsArray;

    QJsonDocument jsonDoc(requestBody);
    QByteArray jsonData = jsonDoc.toJson();

    qDebug() << "🔄 Envoi de la requête à :" << request.url().toString();
    qDebug() << "📤 Données envoyées :" << jsonData;

    QNetworkReply *reply = networkManager.post(request, jsonData);
    connect(reply, &QNetworkReply::finished, this, [reply]() {
        QByteArray responseData = reply->readAll();
        qDebug() << "📥 Données reçues :" << responseData;
        reply->deleteLater();
    });
}

void GeminiAI::handleNetworkReply(QNetworkReply *reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        qDebug() << "📄 Réponse brute :" << responseData;

        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
        if (!jsonResponse.isObject()) {
            qWarning() << "⚠️ Réponse JSON invalide";
            emit responseReceived("Invalid response from Gemini.");
        } else {
            QJsonObject jsonObject = jsonResponse.object();
            qDebug() << "✅ JSON analysé :" << jsonObject;

            if (jsonObject.contains("candidates")) {
                QJsonArray candidates = jsonObject["candidates"].toArray();
                if (!candidates.isEmpty() && candidates[0].isObject()) {
                    QJsonObject firstCandidate = candidates[0].toObject();
                    if (firstCandidate.contains("content") && firstCandidate["content"].isObject()) {
                        QJsonObject content = firstCandidate["content"].toObject();
                        if (content.contains("parts") && content["parts"].isArray()) {
                            QJsonArray parts = content["parts"].toArray();
                            if (!parts.isEmpty() && parts[0].isObject()) {
                                QJsonObject part = parts[0].toObject();
                                if (part.contains("text")) {
                                    QString aiResponse = part["text"].toString();
                                    emit responseReceived(aiResponse);
                                    reply->deleteLater();
                                    return;
                                }
                            }
                        }
                    } else {
                        qWarning() << "Clé 'content' ou 'parts' manquante";
                    }
                } else {
                    qWarning() << "Aucun candidat valide trouvé";
                }
            } else {
                qWarning() << "Clé 'candidates' manquante";
            }
        }
    } else {
        qWarning() << "🌐 Erreur réseau :" << reply->errorString();
        emit responseReceived("Erreur lors de la requête vers Gemini.");
    }

    reply->deleteLater();
}
