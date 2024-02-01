// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef RESTACCESSMANAGER_H
#define RESTACCESSMANAGER_H

#include <QMap>
#include <QMutex>
#include <QNetworkAccessManager>
#include <QObject>
#include <QString>

#include <optional>

class RestAccessManager : public QObject
{
    Q_OBJECT
    struct AuthHeader
    {
        QString header;
        QString token;
    };

    public:
        explicit RestAccessManager(const QString &host, quint16 port);
        ~RestAccessManager();

        bool isAuthorized();

        void setUrl(QString Url, uint Port = 80);
        bool submitCredentials(const QString &user, const QString pass);

        QJsonArray bookings(QDate start, QDate end);

        using QObject::event;
        QJsonObject event(int id);

    signals:
        void authSuccess(bool isSuccess);
        void requestStarted();
        void requestFinished();

    private slots:


        void requestError(QNetworkReply *reply);
        //void requestTimeout();

        void handleReply(QNetworkReply *reply);
        bool getCredentials(QNetworkReply *reply);

    private:
        void setAuthorizationHeader(const QString &key, const QString &value);

    private:

        enum {None, Authorization, EventFetch, EventPush, MaterialFetch, MaterialPush};

        int mStatus;

        QString baseAPI;

        std::optional<AuthHeader> authHeader;
        QNetworkAccessManager manager;
        mutable QMutex eventsMtx;
};

#endif // RESTACCESSMANAGER_H
