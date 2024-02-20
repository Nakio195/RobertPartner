#include "Item.h"

Item::Item()
{
    id  = ItemID();
    name = QString();
    reference = QString();
    description = QString();
    isUnitary = false;
}

Item::Item(QJsonObject json)
{
    id  = ItemID();
    name = QString();
    reference = QString();
    description = QString();
    isUnitary = false;

    for(auto&& key : json.keys())
    {
        if(key == QString("id"))
            id = ItemID(json[key].toInt());
        if(key == QString("title"))
            name = json[key].toString();
        if(key == QString("reference"))
            reference = json[key].toString();;
        if(key == QString("description"))
            description = json[key].toString();;
    }
}


bool Item::operator==(const Item &other) const
{
    return id == other.id;
}

// bool Item::operator==(const QString &other) const
// {
//     return reference == other;
// }

// bool Item::operator==(const int &other) const
// {
//     return id == other;
// }

bool Item::operator<(const Item &other) const
{
    return id.id < other.id.id;
}

bool Item::operator>(const Item &other) const
{
    return id.id > other.id.id;
}


QJsonObject Item::toJson() const
{
    return QJsonObject{ { "id", id.id }, { "name", name }, { "reference", reference }, {"description", description} };
}
