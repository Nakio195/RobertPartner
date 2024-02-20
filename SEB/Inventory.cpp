#include "Inventory.h"

Inventory::Inventory() {}


void Inventory::perform(InventoryAction *action)
{
    if(list.contains(ItemID(action->id.id)) && action->qty!= 0)
    {
        action->inventory = this;
        action->setText(QString::number(action->qty) + "\t| " + list[action->id].name());
        mActions.push(action);
    }
}

QUndoStack* Inventory::getUndoStack()
{
    return &mActions;
}

void Inventory::addLine(const InventoryItem &item)
{
    list.insert(item.item.id, item);
}


InventoryItem Inventory::item(ItemID id)
{
    if(list.contains(ItemID(id.id)))
    {
        if(list[id].isUnitary())
        {
            for(auto &unit : list[id].units)
            {
                if(unit.id() == id)
                    return unit;
            }

            return InventoryItem();
        }
        else
            return list[id];
    }

    else
        return InventoryItem();
}
