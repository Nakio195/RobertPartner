// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef RESTACCESSMANAGER_H
#define RESTACCESSMANAGER_H

#include <QMap>
#include <QMutex>
#include <QNetworkAccessManager>
#include <QObject>
#include <QString>
#include <QTextEdit>

#include <optional>

#include "Event.h"
#include "SEB/Inventory.h"

class RestAccessManager : public QObject
{
    Q_OBJECT
    struct AuthHeader
    {
        QString header;
        QString token;
    };

    public:
        explicit RestAccessManager(const QString &host, quint16 port, QTextEdit* logArea);
        ~RestAccessManager();

        bool isAuthorized();

        void setUrl(QString Url, uint Port = 80);
        bool submitCredentials(const QString &user, const QString pass);

        QList<Event> bookings(QDate start, QDate end);

        using QObject::event;
        Event event(int id);
        Event returnMaterial(Event event, const Inventory &inventory);

    signals:
        void authSuccess(bool isSuccess);
        void requestStarted();
        void requestFinished();
        void requestStateChanged(bool state);
        void dataValidityChanged(bool state);

    private slots:
        void requestError(QNetworkReply *reply);

    private:
        void setAuthorizationHeader(const QString &key, const QString &value);
        QNetworkReply* get(QNetworkRequest *request);
        QNetworkReply* put(QNetworkRequest *request, QJsonDocument parameters);
        QNetworkReply* post(QNetworkRequest *request, QJsonDocument parameters);

        void log(QNetworkReply *reply);
        void log(QString text);

    private:

        int mStatus;

        QString baseAPI;

        std::optional<AuthHeader> authHeader;
        QNetworkAccessManager manager;
        mutable QMutex eventsMtx;

        QTextEdit *logTxt;
};

#endif // RESTACCESSMANAGER_H
