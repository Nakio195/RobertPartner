#ifndef INVENTORYITEM_H
#define INVENTORYITEM_H

#include <QMap>

#include "Item.h"

enum List{Expected, Departed, Return, Fault, size};


class InventoryItem
{
    public:
        InventoryItem();
        bool operator==(const InventoryItem& other) const;

        qint64 quantity(List type) const;
        void set(List type, qint64 value);

        Item item;

        ItemID id() const;
        QString name() const;
        QString reference() const;

        bool isUnitary() const;

        QList<InventoryItem> units;

    private:
        QMap<int, qint64> mQty;
};

#endif // INVENTORYITEM_H
