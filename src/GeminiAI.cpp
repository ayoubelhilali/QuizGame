#include "headers/geminiai.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkRequest>
#include <QDebug>
#include <QRegularExpression>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>

GeminiAI::GeminiAI(QObject *parent) : QObject(parent) {
    apiKey = "AIzaSyD5VEqZ_JGri9TP4OFzDI0rarUgpYWy_U0";

    // Ensure the database is opened
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isValid() || !db.isOpen()) {
        qDebug() << "❌ Database is not open or invalid. Attempting to open.";
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("questions.db");

        if (!db.open()) {
            qDebug() << "❌ Failed to open DB:" << db.lastError().text();
        } else {
            qDebug() << "✅ Database opened successfully at:" << db.databaseName();
        }
    }
}

QString removeControlCharacters(const QString& str) {
    QString result;
    for (QChar c : str) {
        ushort code = c.unicode();
        if (code >= 0x20) {
            result += c;
        }
    }
    return result;
}

void GeminiAI::setApiKey(const QString &key) {
    apiKey = key;
}

void GeminiAI::askQuestion(const QString &question, QString domain) {
    qDebug() << "----------  askQuestion executed ------------";
    QUrl url(QString("https://generativelanguage.googleapis.com/v1beta/models/gemini-2.0-flash:generateContent?key=%1").arg(apiKey));
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

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


    QNetworkReply *reply = networkManager.post(request, jsonData);
    connect(reply, &QNetworkReply::errorOccurred, this, [this, reply](QNetworkReply::NetworkError error) {
        qWarning() << "🌐 Erreur réseau :" << reply->errorString();
    });

    connect(reply, &QNetworkReply::finished, this, [this, reply,domain]() {
        if (reply->error() != QNetworkReply::NoError) {
            emit responseReceived("Erreur lors de la requête vers Gemini.");
            reply->deleteLater();
            return;
        }

        QByteArray responseData = reply->readAll();
        qDebug() << "📄 Réponse brute :" << responseData;
        reply->deleteLater();

        processTextResponse(responseData);
        processJsonQuestionsResponse(responseData,domain);
    });
}

void GeminiAI::processTextResponse(const QByteArray &responseData) {
    qDebug() << "----------  processTextResponse executed ------------";
    QJsonParseError parseError;
    QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        qDebug() << "❌ Failed to parse response:" << parseError.errorString();
        emit responseReceived("Invalid response format from Gemini.");
        return;
    }

    if (!jsonResponse.isObject()) {
        qWarning() << "⚠️ Réponse JSON invalide";
        emit responseReceived("Invalid response from Gemini.");
        return;
    }

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
                            return;
                        }
                    }
                }
            }
        }
    }

    qWarning() << "⚠️ Structure de réponse Gemini inattendue";
    emit responseReceived("Couldn't process Gemini's response properly.");
}

void GeminiAI::processJsonQuestionsResponse(const QByteArray &responseData, const QString &domain) {
    qDebug() << "----------  processJsonQuestionsResponse executed ------------";

    QString extractedJsonText = extractJsonTextFromGemini(responseData);
    if (extractedJsonText.isEmpty()) return;

    QJsonParseError parseError;
    QJsonDocument innerDoc = QJsonDocument::fromJson(extractedJsonText.toUtf8(), &parseError);
    if (parseError.error != QJsonParseError::NoError) return;

    parseAndStoreQuestions(innerDoc.toJson(), domain);
}
QString GeminiAI::extractJsonTextFromGemini(const QByteArray &responseData) {
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(responseData, &error);
    if (error.error != QJsonParseError::NoError || !doc.isObject()) return {};

    QJsonObject obj = doc.object();
    QJsonArray candidates = obj.value("candidates").toArray();
    if (candidates.isEmpty()) return {};

    QJsonObject content = candidates[0].toObject().value("content").toObject();
    QJsonArray parts = content.value("parts").toArray();
    if (parts.isEmpty()) return {};

    QString rawText = parts[0].toObject().value("text").toString();
    return cleanJsonText(rawText);
}

QString GeminiAI::cleanJsonText(const QString &rawText) {
    QString text = rawText;
    if (text.startsWith("```json")) text.remove(0, 7);
    if (text.endsWith("```")) text.chop(3);
    return QString::fromUtf8(removeControlCharacters(text).toUtf8());
}


void GeminiAI::parseAndStoreQuestions(const QByteArray& response, QString domain) {
    qDebug() << "----------  parseAndStoreQuestions executed ------------";
    QJsonDocument doc = QJsonDocument::fromJson(response);

    if (!doc.isArray()) {
        qDebug() << "❌ Expected a JSON array";
        return;
    }

    QJsonArray questionsArray = doc.array();
    QSqlDatabase db = QSqlDatabase::database();

    if (!db.transaction()) {
        qDebug() << "❌ Could not start transaction:" << db.lastError().text();
        return;
    }

    bool hasError = false;
    int insertCount = 0;

    for (const QJsonValue& val : questionsArray) {
        QJsonObject obj = val.toObject();
        QString question = obj["question"].toString();
        QJsonArray options = obj["options"].toArray();
        QString correct = obj["correct_answer"].toString();

        if (options.size() != 4) {
            qDebug() << "❌ Skipping invalid options count";
            continue;
        }

        QSqlQuery query(db);
        query.prepare("INSERT INTO mcq_questions (question,domain, option1, option2, option3, option4, correct_answer) "
                      "VALUES (:q, :d,:o1, :o2, :o3, :o4, :c)");

        query.bindValue(":q", question);
        query.bindValue(":d", domain);
        query.bindValue(":o1", options[0].toString());
        query.bindValue(":o2", options[1].toString());
        query.bindValue(":o3", options[2].toString());
        query.bindValue(":o4", options[3].toString());
        query.bindValue(":c", correct);

        if (!query.exec()) {
            qDebug() << "❌ Insert failed:" << query.lastError().text();
            qDebug() << "❌ Failed query:" << query.lastQuery();
            hasError = true;
            break;
        } else {
            insertCount++;
            qDebug() << "✅ Question inserted";
        }
    }

    if (hasError) {
        if (!db.rollback()) {
            qDebug() << " Rollback failed:" << db.lastError().text();
        }
        qDebug() << " Transaction rolled back due to errors";
    } else {
        if (!db.commit()) {
            qDebug() << " Commit failed:" << db.lastError().text();
        } else {
            qDebug() << " Successfully committed" << insertCount << "questions to database";
        }
    }
}
QVector<QJsonObject> GeminiAI::getQuestionsFromDB() {
    qDebug() << "----------  getQuestionfromDb executed ------------";
    QVector<QJsonObject> questions;

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qDebug() << "❌ Base de données non ouverte.";
        return questions;
    }

    QSqlQuery query("SELECT question,domain, option1, option2, option3, option4, correct_answer FROM mcq_questions", db);
    if (!query.exec()) {
        qDebug() << "❌ Erreur lors de l'exécution de la requête SELECT :" << query.lastError().text();
        return questions;
    }

    while (query.next()) {
        QJsonObject questionObj;
        questionObj["question"] = query.value(0).toString();

        QJsonArray options;

        options.append(query.value(2).toString());
        options.append(query.value(3).toString());
        options.append(query.value(4).toString());
        options.append(query.value(5).toString());

        questionObj["options"] = options;
        questionObj["domain"] = query.value(1).toString();
        questionObj["correct_answer"] = query.value(6).toString();

        questions.append(questionObj);
    }

    qDebug() << "✅" << questions.size() << "questions récupérées depuis la base de données.";
    return questions;
}
