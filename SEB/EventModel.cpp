// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "EventModel.h"
#include "Event.h"

#include <QJsonArray>

EventModel::EventModel(RestAccessManager *manager, QObject *parent)    : QAbstractTableModel(parent)
{
    if(manager == nullptr)
        return;

    api = manager;

    Event event;

    event.id = 123456;
    event.name = QString("Coucou");
    event.start = QDate(2024, 01, 21);
    event.end = QDate(2024, 01, 29);

    events.append(event);
    events.append(event);
    events.append(event);
    events.append(event);
    events.append(event);
}

EventModel::~EventModel() = default;

int EventModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return events.size();
}

int EventModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return Event::size();
}

QVariant EventModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Orientation::Horizontal)
    {
        if(role == Qt::DisplayRole)
        {
            switch (section)
            {
                case 0:
                    return QVariant("id"); // ID is not shown
                case 1:
                    return QVariant("Nom");
                case 2:
                    return QVariant("Parti");
                case 3:
                    return QVariant("Retourné");
                case 4:
                    return QVariant("Date début");
                case 5:
                    return QVariant("Date fin");
            }
        }
    }
    return QVariant();
}

QVariant EventModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= events.count())
        return QVariant();

    if(role == Qt::CheckStateRole)
    {
        if(index.column() == 2)
        {
            if(events[index.row()].departed == true)
                return Qt::Checked;
            else
                return Qt::Unchecked;
        }

        if(index.column() == 3)
        {
            if(events[index.row()].returned == true)
                return Qt::Checked;
            else
                return Qt::Unchecked;
        }
    }

    if(role == Qt::TextAlignmentRole)
        if(index.column() == 2 || index.column() == 3)
            return Qt::AlignCenter;

    if(role == Qt::DisplayRole)
    {
        const Event &event = events.at(index.row());
        QVariant ret;

        switch (index.column())
        {
            case EventRoles::IdRole:
                ret = event.id;
                break;

            case EventRoles::NameRole:
                ret = event.name;
                break;

            // case EventRoles::DepartedRole:
            //     ret = event.departed;
            //     break;

            // case EventRoles::ReturnedRole:
            //     ret = event.returned;
            //     break;

            case EventRoles::StartRole:

                ret = QLocale::system().toString(event.start, "d MMMM");
                break;

            case EventRoles::EndRole:
                ret = QLocale::system().toString(event.end, "d MMMM");
                break;
        }

        return ret.toString();
    }

    return QVariant();
}

Qt::ItemFlags EventModel::flags(const QModelIndex &index) const
{
    if(index.column() == 2 || index.column() == 3)
        return QAbstractTableModel::flags(index);

    else
        return QAbstractTableModel::flags(index);
}

Event EventModel::getEvent(const QModelIndex &event)
{
    if(!event.isValid())
        qDebug("Invalid event selection");

    return events[event.row()];
}


Q_INVOKABLE void EventModel::fetch(QDate start, QDate end)
{
    QList<Event> tmpEvents =  api->bookings(start, end);

    beginResetModel();
    events.clear();
    events = tmpEvents;
    endResetModel();
}


void EventModel::showDates(bool state)
{
    datesVisible = state;
}

