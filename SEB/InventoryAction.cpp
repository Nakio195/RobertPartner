#include "InventoryAction.h"
#include "SEB/Inventory.h"

InventoryAction::InventoryAction(ItemID ref, qint64 q, List list) : QUndoCommand()
{
    qty = q;
    id = ref;
    listType = list;
}

InventoryAction::InventoryAction(InventoryItem line, qint64 q, List list) : QUndoCommand()
{
    qty = q;
    id = line.item.id;
    listType = list;
}

InventoryAction::InventoryAction(const InventoryAction &other) : QUndoCommand()
{
    qty = other.qty;
    id = other.id;
    listType = other.listType;
}

InventoryAction& InventoryAction::operator=(const InventoryAction &other)
{
    qty = other.qty;
    id = other.id;
    listType = other.listType;

    return *this;
}


void InventoryAction::undo()
{
    int oldQty = qty;
    qty = -qty;

    // Check if item exist in inventory by escaping unit id
    if(inventory->list.contains(ItemID(id.id)) && qty != 0)
    {
        InventoryItem *item = nullptr;

        if(inventory->list[ItemID(id.id)].isUnitary())
        {
            for(int i = 0; i < inventory->list[ItemID(id.id)].units.size(); i++)
            {
                if(inventory->list[ItemID(id.id)].units[i].id() == id)
                    item = &inventory->list[ItemID(id.id)].units[i];
            }
        }

        else
            item = &inventory->list[id];

        if(item != nullptr)
        {
            int value = item->quantity(listType);
            int result = value + qty;

            if(result < 0)
                qty -= result;
            if(result > item->quantity(List::Expected))
                qty -= (result - item->quantity(List::Expected));

            result = result < 0 ? 0 : result > item->quantity(List::Expected) ? item->quantity(List::Expected) : result;

            item->set(listType, result);
            emit inventory->itemChanged(*item);
        }
    }
    else
        setObsolete(true);

    qty = oldQty;
}

void InventoryAction::redo()
{
    if(inventory->list.contains(ItemID(id.id)) && qty != 0)
    {
        InventoryItem *item = nullptr;

        if(inventory->list[ItemID(id.id)].isUnitary())
        {
            for(int i = 0; i < inventory->list[ItemID(id.id)].units.size(); i++)
            {
                if(inventory->list[ItemID(id.id)].units[i].id() == id)
                    item = &inventory->list[ItemID(id.id)].units[i];
            }
        }

        else
            item = &inventory->list[id];

        if(item != nullptr)
        {
            int value = item->quantity(listType);
            int result = value + qty;

            if(result < 0)
                qty -= result;
            if(result > item->quantity(List::Expected))
                qty -= (result - item->quantity(List::Expected));

            result = result < 0 ? 0 : result > item->quantity(List::Expected) ? item->quantity(List::Expected) : result;

            if((qty > 0 && value != item->quantity(List::Expected)) || (qty < 0 && value != 0))
            {
                item->set(listType, result);
                emit inventory->itemChanged(*item);
            }
            else
                setObsolete(true);
        }
    }
    else
        setObsolete(true);
}
