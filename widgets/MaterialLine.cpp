#include "MaterialLine.h"
#include "ui_MaterialLine.h"

MaterialLine::MaterialLine(QVBoxLayout *list, Material &mat, int qty, int qtyDep, int qtyRet, int qtyFau) : QWidget(list->parentWidget()), ui(new Ui::MaterialLine)
{
    ui->setupUi(this);

    if(list != nullptr)
        list->addWidget(this);

    archived = false;
    changed = false;

    material = mat;
    expectedQty = qty;

    ui->returnedQty->setRange(0, expectedQty);
    ui->faultQty->setRange(0, expectedQty);

    ui->name->setText(material.name);
    ui->reference->setText(material.reference);
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

Material MaterialLine::getMaterial()
{
    return material;
}

bool MaterialLine::isArchived()
{
    return archived;
}


bool MaterialLine::hasChanged()
{
    bool b = changed;
    changed = false;
    return b;
}

QString MaterialLine::reference()
{
    return material.reference;
}

void MaterialLine::returnQtyChanged(int value)
{
    changed = true;
    material.qtyReturned = value;

    if(value == expectedQty)
    {
        this->setAutoFillBackground(true);
        ui->btnArchive->show();
        ui->btnPlusReturned->setEnabled(false);
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


    if(value < ui->faultQty->value())
        ui->faultQty->setValue(value);


    emit qtyChange();
}

void MaterialLine::faultQtyChanged(int value)
{
    changed = true;

    material.qtyFault = value;

    if(value == 0)
    {
        ui->btnMinusFault->setEnabled(false);

        QPalette green;
        green.setColor(QPalette::Window, QColor(41, 60, 22));
        this->setPalette(green);
    }
    else
    {
        ui->btnMinusFault->setEnabled(true);
        QPalette red;
        red.setColor(QPalette::Window, QColor(55, 34, 22));
        this->setPalette(red);
        this->setAutoFillBackground(true);

        if(value == expectedQty)
            ui->btnPlusFault->setEnabled(false);
        else
            ui->btnPlusFault->setEnabled(true);
    }

    // More faulty material than returned material
    if(value > ui->returnedQty->value())
        ui->returnedQty->setValue(value);

    emit qtyChange();

}

void MaterialLine::archiveLine()
{
    this->hide();
    archived = true;
    emit archive(material);
}
