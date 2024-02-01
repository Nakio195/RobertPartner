#include "Material.h"

Material::Material()
{
    id  = -1;
    name = QString();
    reference = QString();
    description = QString();

    qtyDeparted  = 0;
    qtyReturned  = 0;
    qtyFault  = 0;
    qty  = 0;
}

Material::Material(QJsonObject json)
{
    id  = -1;
    name = QString();
    reference = QString();
    description = QString();

    qtyDeparted  = 0;
    qtyReturned  = 0;
    qtyFault  = 0;
    qty  = 0;

    for(auto&& key : json.keys())
    {
        if(key == QString("id"))
            id = json[key].toInt();
        if(key == QString("title"))
            name = json[key].toString();
        if(key == QString("reference"))
            reference = json[key].toString();;
        if(key == QString("description"))
            description = json[key].toString();;
    }
}


bool Material::operator==(const Material &other) const
{
    return id == other.id;
}

bool Material::operator==(const int &other) const
{
    return id == other;
}

bool Material::operator<(const Material &other) const
{
    return id < other.id;
}

bool Material::operator>(const Material &other) const
{
    return id > other.id;
}


QJsonObject Material::toJson() const
{
    return QJsonObject{ { "id", id }, { "name", name }, { "reference", reference }, {"description", description} };
}
