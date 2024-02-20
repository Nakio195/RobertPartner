#include "MaterialLine.h"
#include "ui_MaterialLine.h"

MaterialLine::MaterialLine(QVBoxLayout *list, const InventoryItem &ln) : QWidget(list->parentWidget()), line(ln), ui(new Ui::MaterialLine)
{
    ui->setupUi(this);

    if(list != nullptr)
        list->addWidget(this);

    archived = false;
    archivable = false;
    changed = false;


    if(line.item.isUnitary)
    {
        ui->reference->setText(QString("Référence : ") + line.reference());
    }

    else
    {
        ui->reference->setText(QString::number(line.item.id.id));
    }

    ui->returnedQty->setRange(0, line.quantity(List::Expected));
    ui->faultQty->setRange(0, line.quantity(List::Expected));

    ui->name->setText(line.item.name);

    ui->expectedQty->setText(QString::number(line.quantity(List::Expected)));
    ui->btnArchive->hide();

    oldRet = line.quantity(List::Return);
    oldFault = line.quantity(List::Fault);

    ui->returnedQty->setValue(line.quantity(List::Return));
    ui->faultQty->setValue(line.quantity(List::Fault));

}

MaterialLine::~MaterialLine()
{
    delete ui;
}

bool MaterialLine::isArchived()
{
    return archived;
}

void MaterialLine::setArchived(bool state)
{
    if(state && !archivable)
        return;

    if(state)
        archiveLine();

    else
    {
        this->show();
        archived = false;
    }
}

bool MaterialLine::hasChanged()
{
    bool b = changed;
    changed = false;
    return b;
}

void MaterialLine::update(const InventoryItem& item)
{

    if(item.id() != line.id())
        return;

    oldRet = item.quantity(List::Return);
    oldFault = item.quantity(List::Fault);

    changed = true;

    ui->returnedQty->blockSignals(true);
    ui->returnedQty->setValue(oldRet);
    ui->returnedQty->blockSignals(false);

    ui->faultQty->blockSignals(true);
    ui->faultQty->setValue(oldFault);
    ui->faultQty->blockSignals(false);

    handleUI();
}

void MaterialLine::returnQtyChanged(int value)
{
    changed = true;

    if(oldRet != value)
    {

        InventoryAction *action = new InventoryAction(line, value - oldRet, List::Return);
        emit valueChanged(action);

        oldRet = value;

        if(value < ui->faultQty->value())
            ui->faultQty->setValue(value);
    }

    handleUI();
}

void MaterialLine::faultQtyChanged(int value)
{
    if(oldFault != value)
    {
        changed = true;

        InventoryAction *action = new InventoryAction(line, value - oldFault, List::Fault);
        emit valueChanged(action);

        oldFault = value;

        // More faulty material than returned material
        if(value > ui->returnedQty->value())
            ui->returnedQty->setValue(value);
    }

    handleUI();
}


void MaterialLine::handleUI()
{
    if(!hasChanged())
        return;

    int valueRet = ui->returnedQty->value();
    int valueFault = ui->faultQty->value();

    //Buttons
    if(valueFault == 0)
        ui->btnMinusFault->setEnabled(false);
    else
        ui->btnMinusFault->setEnabled(true);

    if(valueFault == line.quantity(List::Expected))
        ui->btnPlusFault->setEnabled(false);
    else
        ui->btnPlusFault->setEnabled(true);

    if(valueRet == 0)
        ui->btnMinusReturned->setEnabled(false);
    else
        ui->btnMinusReturned->setEnabled(true);

    if(valueRet == line.quantity(List::Expected))
        ui->btnPlusReturned->setEnabled(false);
    else
        ui->btnPlusReturned->setEnabled(true);


    //Archive
    if(valueRet == line.quantity(List::Expected))
    {
        archivable = true;
        ui->btnArchive->show();
    }
    else
    {
        archivable = false;
        ui->btnArchive->hide();
    }

    //Color
    if(valueRet == line.quantity(List::Expected))
    {
        QPalette green;
        green.setColor(QPalette::Window, QColor(89, 170, 94));
        this->setPalette(green);
        this->setAutoFillBackground(true);
    }

    if(valueFault != 0)
    {
        QPalette red;
        red.setColor(QPalette::Window, QColor(170, 89, 94));
        this->setPalette(red);
        this->setAutoFillBackground(true);
    }

    if(valueFault == 0 && valueRet != line.quantity(List::Expected))
        this->setAutoFillBackground(false);

}

void MaterialLine::archiveLine()
{
    this->hide();
    archived = true;
}
