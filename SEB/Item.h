#ifndef ITEM_H
#define ITEM_H

#include <QJsonObject>

typedef int UnitID;

struct Unit
{
    UnitID id;
    QString reference;
};

struct ItemID
{
    int id;
    UnitID unit;

    explicit ItemID()
    {
        id = -1;
        unit = -1;
    }

    explicit ItemID(int i, int u = -1)
    {
        id = i;
        unit = u;
    }

    ItemID(const ItemID& other)
    {
        id = other.id;
        unit = other.unit;
    }

    // void operator=(const int &value)
    // {
    //     id = value;
    // }

    void operator=(const ItemID &other)
    {
        id = other.id;
        unit = other.unit;
    }

    bool operator<(const ItemID &other) const
    {
        if(id == other.id)
            return unit < other.unit;
        else
            return id < other.id;
    }

    bool operator>(const ItemID &other) const
    {
        return id < other.id;
    }

    bool operator!=(const ItemID &other) const
    {
        return (id != other.id) || (unit != other.unit);
    }

    // bool operator!=(const int &other) const
    // {
    //     return id != other;
    // }

    bool operator==(const ItemID &other) const
    {
        return (id == other.id && unit == other.unit);
    }

    // bool operator==(const int &other) const
    // {
    //     return id == other;
    // }
};


class Item
{
    public :
        explicit Item();
        explicit Item(QJsonObject json);

        ItemID id;
        QString name;
        QString reference;
        QString description;

        bool isUnitary;

        QJsonObject toJson() const;
        bool operator==(const Item &other) const;
        bool operator==(const QString &other) const;
        bool operator==(const int &other) const;
        bool operator<(const Item &other) const;
        bool operator>(const Item &other) const;

        static constexpr qsizetype size() { return 4; }
};

#endif // ITEM_H
