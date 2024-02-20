// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef TYPES_H
#define TYPES_H

#include <QJsonObject>
#include <QJsonArray>
#include <QString>
#include <QDate>

#include "InventoryItem.h"

class Event
{
    public :
        Event();
        Event(QJsonObject json);

        QJsonObject toJson() const;
        bool operator==(const Event &other) const;

        static constexpr qsizetype size() { return 6; }

        Item* get(QString ref);

    private:
        void parseItems(const QJsonArray &mat);

    public:
        qint64 id;
        QString name;
        QDate start;
        QDate end;

        bool departed;
        bool returned;

        QList<InventoryItem> items;
};

#endif // TYPES_H
