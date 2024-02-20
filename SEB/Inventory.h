#ifndef INVENTORY_H
#define INVENTORY_H

#include <QString>
#include <QMap>
#include <QUndoStack>

#include "InventoryItem.h"
#include "InventoryAction.h"


class Inventory : public QObject
{
    Q_OBJECT
    public:
        explicit Inventory();

        void addLine(const InventoryItem &item);

        QUndoStack* getUndoStack();

        InventoryItem item(ItemID id);

    signals:
        void itemChanged(const InventoryItem& item);

    public slots:
        void perform(InventoryAction *action);


    public:
        QMap<ItemID, InventoryItem> list;

    private:
        QUndoStack mActions;
};

#endif // INVENTORY_H
