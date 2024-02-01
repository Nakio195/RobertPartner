#ifndef MATERIAL_H
#define MATERIAL_H

#include <QJsonObject>

class Material
{
    public :
        Material();
        Material(QJsonObject json);

        qint64 id;
        QString name;
        QString reference;
        QString description;

        uint qtyDeparted;
        uint qtyReturned;
        uint qtyFault;
        uint qty;

        QJsonObject toJson() const;
        bool operator==(const Material &other) const;
        bool operator<(const Material &other) const;
        bool operator>(const Material &other) const;

        static constexpr qsizetype size() { return 4; }
};

#endif // MATERIAL_H
