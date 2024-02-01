#ifndef MATERIALLINE_H
#define MATERIALLINE_H

#include <QWidget>
#include "SEB/Material.h"
#include <QVBoxLayout>


namespace Ui {
class MaterialLine;
}

class MaterialLine : public QWidget
{
    Q_OBJECT

    public:
        explicit MaterialLine(QVBoxLayout *list, Material &mat, int qty, int qtyDep = 0, int qtyRet = 0, int qtyFau = 0);
        ~MaterialLine();

        QString reference();


    signals:
        void archive();

    public slots:
        void returnQtyChanged(int value);
        void faultQtyChanged(int value);
        void archiveLine();

    private:
        Ui::MaterialLine *ui;

        Material mMaterial;
        int expectedQty;
};

#endif // MATERIALLINE_H
