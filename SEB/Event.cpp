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
        if(key == QString("start_date"))
            end = QDate::fromString(json[key].toString().sliced(0, 10), "yyyy-MM-dd");
        if(key == QString("materials"))
            getMaterials(json[key].toArray());

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


void Event::getMaterials(QJsonArray mat)
{
    materials.clear();

    for(auto json : mat)
    {
        Material material;
        QJsonObject o = json.toObject();

        for(auto&& key : o.keys())
        {
            if(key == QString("id"))
                material.id = o[key].toInt();
            if(key == QString("name"))
                material.name = o[key].toString();
            if(key == QString("reference"))
                material.reference = o[key].toString();
            if(key == QString("description"))
                material.description = o[key].toString();

            if(key == QString("pivot"))
            {
                QJsonObject oo = o[key].toObject();
                if(oo.contains("quantity"))
                    material.qty = oo["quantity"].toInt();
                if(oo.contains("quantity_departed"))
                    material.qtyDeparted =  oo["quantity_departed"].toInt();
                if(oo.contains("quantity_returned_broken"))
                    material.qtyFault = oo["quantity_returned_broken"].toInt();
                if(oo.contains("quantity_returned"))
                    material.qtyReturned = oo["quantity_returned"].toInt();
            }
        }

        materials.append(material);
    }
}
