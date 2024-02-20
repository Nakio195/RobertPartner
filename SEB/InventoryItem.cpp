#include "InventoryItem.h"

InventoryItem::InventoryItem()
{
    for(int i = List::Departed; i < List::size; i++)
        mQty[i] = 0;
}

bool InventoryItem::operator==(const InventoryItem& other) const
{
    return other.item == item;
}


qint64 InventoryItem::quantity(List type) const
{
    return mQty[type];
}

void InventoryItem::set(List type, qint64 value)
{
    mQty[type] = value;
}


ItemID InventoryItem::id() const
{
    return item.id;
}

QString InventoryItem::name() const
{
    return item.name;
}

QString InventoryItem::reference() const
{
    return item.reference;
}

bool InventoryItem::isUnitary() const
{
    return item.isUnitary;
}
