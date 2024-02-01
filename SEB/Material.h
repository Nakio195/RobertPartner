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

        qint64 qtyDeparted;
        qint64 qtyReturned;
        qint64 qtyFault;
        qint64 qty;

        QJsonObject toJson() const;
        bool operator==(const Material &other) const;
        bool operator==(const int &other) const;
        bool operator<(const Material &other) const;
        bool operator>(const Material &other) const;

        static constexpr qsizetype size() { return 4; }
};

#endif // MATERIAL_H
