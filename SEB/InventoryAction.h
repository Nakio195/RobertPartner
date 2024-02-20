#ifndef INVENTORYACTION_H
#define INVENTORYACTION_H

#include <QString>
#include <QUndoCommand>
#include "InventoryItem.h"


class Inventory;

class InventoryAction : public QUndoCommand
{
    public:
        explicit InventoryAction(ItemID id, qint64 q = 0, List list = List::Return);
        explicit InventoryAction(InventoryItem line, qint64 q = 0, List list = List::Return);
        InventoryAction(const InventoryAction &other);
        InventoryAction &operator=(const InventoryAction &other);

        void undo() override;
        void redo() override;

    public:
        qint64 qty;
        ItemID id;
        List listType;
        Inventory *inventory;

};
#endif // INVENTORYACTION_H
