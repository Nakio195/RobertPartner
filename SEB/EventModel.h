// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef EVENTMODEL_H
#define EVENTMODEL_H

#include "Event.h"
#include "RESTManager.h"

#include <QAbstractItemModel>
#include <QMutex>
#include <QSharedPointer>
#include <QtQml/qqml.h>

class EventModel : public QAbstractTableModel
{
        Q_OBJECT
        QML_ELEMENT
    public:
        enum EventRoles { IdRole = 0, NameRole, DepartedRole, ReturnedRole,  StartRole, EndRole};
        Q_ENUM(EventRoles)

        explicit EventModel(RestAccessManager *manager, QObject *parent = nullptr);
        ~EventModel();

        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        int columnCount(const QModelIndex &parent = QModelIndex()) const override;
        QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        Qt::ItemFlags flags(const QModelIndex &index) const override;

        Event getEvent(const QModelIndex &event);


    signals:

    public slots:
        Q_INVOKABLE void fetch(QDate start, QDate end = QDate(0, 0, 0));
        void showDates(bool state);

    private:
        RestAccessManager *api;
        QList<Event> events;

        bool datesVisible;
};

#endif
