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


RestAccessManager::RestAccessManager(const QString &host, quint16 port, QTextEdit *logArea)
{
    baseAPI = host + QString("/api");

    manager.connectToHost(host, port);
    manager.setAutoDeleteReplies(true);
    logTxt = logArea;

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

    QNetworkReply* reply = post(&request, QJsonDocument(creds));

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

void RestAccessManager::log(QNetworkReply* reply)
{
    int httpCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    if(httpCode < 299 && httpCode >= 200)
        logTxt->append("Request success : " + QString::number(httpCode));

    else
    {
        logTxt->append("Request failed : " + QString::number(httpCode) + QString("\n"));
        logTxt->append(reply->readAll());
    }
}

void RestAccessManager::log(QString text)
{
    logTxt->append(text);
}

QNetworkReply* RestAccessManager::get(QNetworkRequest *request)
{
    request->setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, "application/json");
    request->setRawHeader(authHeader->header.toUtf8(), "Bearer " + authHeader->token.toUtf8());
    request->setHttp2Configuration(QHttp2Configuration());

    log("Sending GET request " + request->url().toDisplayString());

    emit requestStarted();
    QEventLoop loop;
    connect(&manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);

    QNetworkReply* reply = manager.get(*request);
    loop.exec();

    emit requestFinished();

    disconnect(&manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);

    log(reply);

    return reply;
}

QNetworkReply* RestAccessManager::put(QNetworkRequest *request, QJsonDocument parameters)
{
    request->setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, "application/json");
    request->setRawHeader(authHeader->header.toUtf8(), "Bearer " + authHeader->token.toUtf8());
    request->setHttp2Configuration(QHttp2Configuration());
    request->setHeader(QNetworkRequest::KnownHeaders::ContentLengthHeader, parameters.toJson(QJsonDocument::Compact).size());

    log("Sending PUT request " + request->url().toDisplayString());
    qDebug() << parameters.toJson(QJsonDocument::Compact);

    emit requestStarted();
    QEventLoop loop;
    connect(&manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);

    QNetworkReply* reply = manager.put(*request, parameters.toJson(QJsonDocument::Compact));
    loop.exec();

    emit requestFinished();

    disconnect(&manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);

    log(reply);

    return reply;
}

QNetworkReply* RestAccessManager::post(QNetworkRequest *request, QJsonDocument parameters)
{
    request->setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, "application/json");
    request->setHttp2Configuration(QHttp2Configuration());
    request->setHeader(QNetworkRequest::KnownHeaders::ContentLengthHeader, parameters.toJson(QJsonDocument::Compact).size());


    QEventLoop loop;
    connect(&manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);

    QNetworkReply* reply = manager.post(*request, parameters.toJson(QJsonDocument::Compact));
    loop.exec();

    disconnect(&manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);

    log(reply);

    return reply;
}


QList<Event> RestAccessManager::bookings(QDate start, QDate end)
{
    QUrl url(baseAPI+QString("/bookings"));
    QUrlQuery parameters;

    parameters.addQueryItem("start", start.toString("yyyy-MM-dd")+QString("+00:00:00"));
    parameters.addQueryItem("end", end.toString("yyyy-MM-dd")+QString("+23:59:59"));
    url.setQuery(parameters);

    auto request = QNetworkRequest(url);
    QNetworkReply *reply = get(&request);

    QList<Event> events;

    const std::optional<QJsonArray> json = byteArrayToJsonArray(reply->readAll());
    if(json)
    {
        QJsonArray data = *json;

        if(!data.isEmpty())
        {
            for(auto o : data)
            {
                if(o.isObject())
                {
                    QJsonObject jsonEvent = o.toObject();

                    events.append(Event(jsonEvent));
                }
            }
        }
    }

    return events;
}



Event RestAccessManager::event(int id)
{
    QUrl url(baseAPI+QString("/events/") + QString::number(id));
    auto request = QNetworkRequest(url);

    QNetworkReply* reply = get(&request);

    Event event;

    const std::optional<QJsonObject> json = byteArrayToJsonObject(reply->readAll());

    if(json)
        event = Event(*json);

    return event;
}


Event RestAccessManager::returnMaterial(Event event, const QList<Material> &materials)
{
    QJsonArray list;

    for(auto mat : materials)
        list.append(QJsonObject{{"actual", mat.qtyReturned}, {"broken", mat.qtyFault}, {"id", mat.id}});

    QUrl url(baseAPI+QString("/events/") + QString::number(event.id) + QString("/return"));
    auto request = QNetworkRequest(url);

    QNetworkReply *reply = put(&request, QJsonDocument(list));

    const std::optional<QJsonObject> json = byteArrayToJsonObject(reply->readAll());

    if(json)
        event = Event(*json);

    return event;

}
