#include "Event.h"

Event::Event()
{
    id = -1;
    name = QString("Invalid");
    start = QDate();
    end = QDate();

    departed = false;
    returned = false;

}

Event::Event(QJsonObject json)
{
    for(auto&& key : json.keys())
    {
        if(key == QString("id"))
            id = json[key].toInt();
        if(key == QString("title"))
            name = json[key].toString();
        if(key == QString("start_date"))
            start = QDate::fromString(json[key].toString().sliced(0, 10), "yyyy-MM-dd");
        if(key == QString("end_date"))
            end = QDate::fromString(json[key].toString().sliced(0, 10), "yyyy-MM-dd");
        if(key == QString("materials"))
            parseItems(json[key].toArray());

        if(key == QString("is_departure_inventory_done"))
            departed = json[key].toBool();
        if(key == QString("is_return_inventory_done"))
            returned = json[key].toBool();
    }
}


bool Event::operator==(const Event &other) const
{
    return name == other.name && id == other.id;
}

QJsonObject Event::toJson() const
{
    return QJsonObject{ { "id", id }, { "name", name }, { "start", start.toString() }, {"end", end.toString()} };
}


void Event::parseItems(const QJsonArray &mat)
{
    items.clear();

    for(auto json : mat)
    {
        InventoryItem line;
        QJsonObject o = json.toObject();

        if(o.contains("id"))
            line.item.id = ItemID(o["id"].toInt());
        if(o.contains("name"))
            line.item.name = o["name"].toString();
        if(o.contains("reference"))
            line.item.reference = o["reference"].toString();
        if(o.contains("description"))
            line.item.description = o["description"].toString();
        if(o.contains("is_unitary"))
            line.item.isUnitary = o["is_unitary"].toBool();

        if(o.contains("pivot"))
        {
            QJsonObject oo = o["pivot"].toObject();

            if(line.isUnitary())
            {
                QMap<int, QString> unitsReference;

                QJsonArray unitsList = o["units"].toArray();
                for(const auto &unitRef : unitsList)
                {
                    QJsonObject unit = unitRef.toObject();
                    unitsReference[unit["id"].toInt()] = unit["reference"].toString();
                }

                if(oo.contains("units_details"))
                {
                    QJsonArray a = oo["units_details"].toArray();

                    for(auto json_unit : a)
                    {
                        InventoryItem unit;
                        unit.item = line.item;
                        unit.set(List::Expected, 1);

                        QJsonObject ooo = json_unit.toObject();

                        if(ooo.contains("id"))
                            unit.item.id.unit = ooo["id"].toInt();
                        if(ooo.contains("is_returned"))
                            unit.set(List::Return, ooo["is_returned"].toBool());
                        if(ooo.contains("is_returned_broken"))
                            unit.set(List::Fault, ooo["is_returned_broken"].toBool());
                        if(unitsReference.contains(unit.item.id.unit))
                            unit.item.reference = unitsReference[unit.item.id.unit];

                        line.units.push_back(unit);
                    }

                }
            }

            else
            {
                if(oo.contains("quantity"))
                    line.set(List::Expected, oo["quantity"].toInt());
                if(oo.contains("quantity_departed"))
                    line.set(List::Departed, oo["quantity_departed"].toInt());
                if(oo.contains("quantity_returned_broken"))
                    line.set(List::Fault, oo["quantity_returned_broken"].toInt());
                if(oo.contains("quantity_returned"))
                    line.set(List::Return, oo["quantity_returned"].toInt());

            }
        }

        items.append(line);
    }
}
