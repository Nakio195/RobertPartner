#include "MaterialLine.h"
#include "ui_MaterialLine.h"

MaterialLine::MaterialLine(QVBoxLayout *list, Material &mat, int qty, int qtyDep, int qtyRet, int qtyFau) : QWidget(list->parentWidget()), ui(new Ui::MaterialLine)
{
    ui->setupUi(this);

    if(list != nullptr)
        list->addWidget(this);

    mMaterial = mat;
    expectedQty = qty;

    ui->returnedQty->setRange(0, expectedQty);
    ui->faultQty->setRange(0, expectedQty);

    ui->name->setText(mMaterial.name);
    ui->reference->setText(mMaterial.name);
    ui->expectedQty->setText(QString::number(expectedQty));
    ui->btnArchive->hide();

    ui->returnedQty->setValue(qtyRet);

    if(qtyRet)
        returnQtyChanged(qtyRet);

    ui->faultQty->setValue(qtyFau);
    if(qtyFau)
        faultQtyChanged(qtyFau);
}

MaterialLine::~MaterialLine()
{
    delete ui;
}

QString MaterialLine::reference()
{
    return mMaterial.reference;
}

void MaterialLine::returnQtyChanged(int value)
{
    if(value == expectedQty)
    {
        ui->btnArchive->show();
        ui->btnPlusReturned->setEnabled(false);

        this->setAutoFillBackground(true);
    }

    else
    {
        this->setAutoFillBackground(false);
        ui->btnArchive->hide();
        ui->btnPlusReturned->setEnabled(true);
    }

    if(value == 0)
        ui->btnMinusReturned->setEnabled(false);
    else
        ui->btnMinusReturned->setEnabled(true);
}

void MaterialLine::faultQtyChanged(int value)
{
    // All materials returned faulty
    if(value == expectedQty)
    {
        ui->btnArchive->show();
        ui->btnPlusFault->setEnabled(false);
        this->setAutoFillBackground(true);
    }
    else
    {
        ui->btnArchive->hide();
        ui->btnPlusReturned->setEnabled(true);
        this->setAutoFillBackground(false);
    }

    if(value == 0)
        ui->btnMinusFault->setEnabled(false);
    else
        ui->btnMinusFault->setEnabled(true);


    // More faulty material than returned material
    if(value > ui->returnedQty->value())
        ui->returnedQty->setValue(value);

}

void MaterialLine::archiveLine()
{
    this->hide();
    emit archive();
}
