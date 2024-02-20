#ifndef MATERIALLINE_H
#define MATERIALLINE_H

#include <QWidget>
#include <QVBoxLayout>
#include "SEB/InventoryAction.h"

namespace Ui {
class MaterialLine;
}

class MaterialLine : public QWidget
{
    Q_OBJECT

    public:
        explicit MaterialLine(QVBoxLayout *list, const InventoryItem &item);
        ~MaterialLine();

        bool isArchived();

        bool hasChanged();

        const InventoryItem line;

    signals:
        void valueChanged(InventoryAction *action);

    public slots:
        void returnQtyChanged(int value);
        void faultQtyChanged(int value);

        void update(const InventoryItem& item);

        void setArchived(bool state);
        void archiveLine();

    private:
        void handleUI();

    private:
        Ui::MaterialLine *ui;

        bool archived;
        bool archivable;
        bool changed;

        qint64 oldRet;
        qint64 oldFault;

};

#endif // MATERIALLINE_H
