// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "RESTManager.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QHttp2Configuration>
#include <QUrlQuery>
#include <QEventLoop>

void debugRequest(QNetworkRequest &request)
{
    qDebug() << "Destination : " << request.url().toString(); //output the url
    const QList<QByteArray>& rawHeaderList(request.rawHeaderList());
    foreach (QByteArray rawHeader, rawHeaderList)
    {
        qDebug() << rawHeader << " : " << request.rawHeader(rawHeader);
    }
}

static std::optional<QJsonObject> byteArrayToJsonObject(const QByteArray &arr)
{
    QJsonParseError err;
    const auto json = QJsonDocument::fromJson(arr, &err);

    if(err.error)
        qDebug() << "Error Parsing JSON : " << err.errorString();

    if(json.isEmpty())
        qDebug() << "Error Parsing JSON : " << "Empty value";

    if(json.isNull())
        qDebug() << "Error Parsing JSON : " << "Null value";

    if(json.isArray())
        qDebug() << "JSON type : " << "Array";

    if(json.isObject())
        qDebug() << "JSON type : " << "Object";

    if (err.error || !json.isObject())
        return std::nullopt;
    return json.object();
}

static std::optional<QJsonArray> byteArrayToJsonArray(const QByteArray &arr)
{
    QJsonParseError err;
    const auto json = QJsonDocument::fromJson(arr, &err);

    if(err.error)
        qDebug() << "Error Parsing JSON : " << err.errorString();

    if(json.isEmpty())
        qDebug() << "Error Parsing JSON : " << "Empty value";

    if(json.isNull())
        qDebug() << "Error Parsing JSON : " << "Null value";

    if(json.isArray())
        qDebug() << "JSON type : " << "Array";

    if(json.isObject())
        qDebug() << "JSON type : " << "Object";

    if (err.error || !json.isArray())
        return std::nullopt;
    return json.array();
}


RestAccessManager::RestAccessManager(const QString &host, quint16 port)
{
    baseAPI = host + QString("/api");

    manager.connectToHost(host, port);
    manager.setAutoDeleteReplies(true);
}

RestAccessManager::~RestAccessManager() = default;

void RestAccessManager::setUrl(QString Url, uint Port)
{
    baseAPI = Url + QString("/api");
}

bool RestAccessManager::submitCredentials(const QString &user, const QString pass)
{

    QJsonObject creds;
    creds["identifier"] = user;
    creds["password"] = pass;

    auto request = QNetworkRequest(QUrl(baseAPI + QString("/session")));
    request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, "application/json");
    request.setHttp2Configuration(QHttp2Configuration());
    request.setHeader(QNetworkRequest::KnownHeaders::ContentLengthHeader, QJsonDocument(creds).toJson(QJsonDocument::Compact).size());


    QEventLoop loop;
    connect(&manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);

    QNetworkReply* reply = manager.post(request, QJsonDocument(creds).toJson(QJsonDocument::Compact));
    loop.exec();

    disconnect(&manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);

    if(reply == nullptr)
        return false;

    const std::optional<QJsonObject> json = byteArrayToJsonObject(reply->readAll());

    if(json)
    {
        QJsonObject d = *json;

        foreach(const QString& key, d.keys())
        {
            QJsonValue value = d.value(key);

            if(key == "token")
                setAuthorizationHeader("Authorization", value.toString());
        }
    }

    if(isAuthorized())
    {
        qDebug() << "Auth success !";
        emit authSuccess(true);
        return true;
    }

    qDebug() << "Auth failed !";
    emit authSuccess(false);

    return false;
}

void RestAccessManager::handleReply(QNetworkReply* reply)
{
    switch(mStatus)
    {
        case Authorization:
            getCredentials(reply);
            break;
    }
}


bool RestAccessManager::getCredentials(QNetworkReply* reply)
{
    // if(reply == nullptr)
    //     return false;

    // const std::optional<QJsonObject> json = byteArrayToJsonObject(reply->readAll());

    // if(json)
    // {
    //     QJsonObject d = *json;

    //     foreach(const QString& key, d.keys())
    //     {
    //         QJsonValue value = d.value(key);

    //         if(key == "token")
    //             setAuthorizationHeader("Authorization", value.toString());
    //     }
    // }

    // if(isAuthorized())
    // {
    //     qDebug() << "Auth success !";
    //     emit authSuccess(true);
    //     return true;
    // }

    // qDebug() << "Auth failed !";
    // emit authSuccess(false);

    return false;
}

void RestAccessManager::requestError(QNetworkReply *reply)
{
    if(reply != nullptr)
    {
        qDebug() << "Server response : " << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute) << " " << reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
        if(reply->error())
            qDebug() << "Creds Reply error !" << reply->errorString() << " " << reply->error();
    }
}

bool RestAccessManager::isAuthorized()
{
    return !authHeader->token.isEmpty();
}

void RestAccessManager::setAuthorizationHeader(const QString &key, const QString &value)
{
    authHeader = AuthHeader{ key, value };
}



QJsonArray RestAccessManager::bookings(QDate start, QDate end)
{
    QUrl url(baseAPI+QString("/bookings"));
    QUrlQuery parameters;

    parameters.addQueryItem("start", start.toString("yyyy-MM-dd")+QString("+00:00:00"));
    parameters.addQueryItem("end", end.toString("yyyy-MM-dd")+QString("+23:59:59"));

    url.setQuery(parameters);

    auto request = QNetworkRequest(url);
    request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, "application/json");
    request.setRawHeader(authHeader->header.toUtf8(), "Bearer " + authHeader->token.toUtf8());
    request.setHttp2Configuration(QHttp2Configuration());

    qDebug() << "Sending GET request " << url;

    emit requestStarted();
    QEventLoop loop;
    connect(&manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);

    QNetworkReply* reply = manager.get(request);
    loop.exec();

    emit requestFinished();

    disconnect(&manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);

    if(reply == nullptr)
        return QJsonArray();

    const std::optional<QJsonArray> json = byteArrayToJsonArray(reply->readAll());

    if(json)
        return *json;

    return QJsonArray();
}



QJsonObject RestAccessManager::event(int id)
{
    QUrl url(baseAPI+QString("/events/") + QString::number(id));

    auto request = QNetworkRequest(url);
    request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, "application/json");
    request.setRawHeader(authHeader->header.toUtf8(), "Bearer " + authHeader->token.toUtf8());
    request.setHttp2Configuration(QHttp2Configuration());

    qDebug() << "Sending GET request " << url;

    emit requestStarted();
    QEventLoop loop;
    connect(&manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);

    QNetworkReply* reply = manager.get(request);
    loop.exec();

    emit requestFinished();

    disconnect(&manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);

    if(reply == nullptr)
        return QJsonObject();

    const std::optional<QJsonObject> json = byteArrayToJsonObject(reply->readAll());

    if(json)
        return *json;

    return QJsonObject();
}














// RestAccessManager::EventsMap RestAccessManager::getEvents() const
// {
//     QMutexLocker lock(&eventsMtx);
//     return contacts;
// }

// void RestAccessManager::updateEvent(const EventEntry &entry)
// {
//     auto request = QNetworkRequest(QUrl(baseAPI));
//     request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, "application/json");

//     if (authHeader)
//         request.setRawHeader(authHeader->key.toLatin1(), authHeader->value.toLatin1());

//     manager.put(request, QJsonDocument(entry.toJson()).toJson(QJsonDocument::Compact));
// }

// void RestAccessManager::readEvents(QNetworkReply *reply)
// {
//     if (reply->error()) {
//         return;
//     }
//     /*
//     const std::optional<QJsonO>bject> array = byteArrayToJsonObject(reply->readAll());

//     if (array)
//     {
//         EventsMap tempEvents;

//         for (const auto &jsonValue : *array)
//         {
//             if (jsonValue.isObject())
//             {
//                 const QJsonObject obj = jsonValue.toObject();

//                 if (obj.contains("id") && obj.contains("name"))
//                     tempEvents.insert(obj.value("id").toInt(),EventEntry{ obj.value("id").toInt(),obj.value("name").toString(),QDate::fromString(obj.value("start").toString()),QDate::fromString(obj.value("end").toString()) });
//             }
//         }
//         {
//             QMutexLocker lock(&eventsMtx);
//             contacts.swap(tempEvents);
//         }
//         emit eventsChanged();
//     } else {
//         this->updateEvents();
//     }
// */
// }

// void RestAccessManager::updateEvents()
// {
//     auto request = QNetworkRequest(baseAPI);
//     request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, "application/json");
//     //manager.get(request);
// }

