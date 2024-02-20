#include "ReturnDialog.h"
#include "ui_ReturnDialog.h"
#include <QAction>

ReturnDialog::ReturnDialog(Event event, RestAccessManager *api, QWidget *parent) : QDialog(parent), ui(new Ui::ReturnDialog), api(api), mEvent(event)
{
    ui->setupUi(this);

    mEvent = Event(api->event(event.id));

    for(int i = 0; i < mEvent.items.size(); i++)
    {
        if(mEvent.items[i].isUnitary())
        {
            mInventory.addLine(mEvent.items[i]);

            for(auto &unit : mEvent.items[i].units)
            {
                MaterialLine *line = new MaterialLine(ui->listLayout, unit);
                connect(line, &MaterialLine::valueChanged, this, &ReturnDialog::inventoryChange);
                connect(&mInventory, &Inventory::itemChanged, line, &MaterialLine::update);
                mControls.insert(unit.item.id, line);
            }
        }

        else
        {
            mInventory.addLine(mEvent.items[i]);
            MaterialLine *line = new MaterialLine(ui->listLayout, mEvent.items[i]);
            connect(line, &MaterialLine::valueChanged, this, &ReturnDialog::inventoryChange);
            connect(&mInventory, &Inventory::itemChanged, line, &MaterialLine::update);
            mControls.insert(mEvent.items[i].item.id, line);
        }


    }

    ui->undoView->setStack(mInventory.getUndoStack());
    ui->eventName->setText(mEvent.name);
    ui->grpHistory->hide();

    this->setWindowTitle("Retour de l'évenement - " + mEvent.name);

    mAutoSave.setInterval(3000);
    connect(&mAutoSave, &QTimer::timeout, this, &ReturnDialog::save);

    barCodeShortcut = new QShortcut(this);
    barCodeShortcut->setKey(QKeySequence(tr("^,#,[")));
    connect(barCodeShortcut, &QShortcut::activated, this, &ReturnDialog::selectBarCode);

    ui->lnFilter->setEnabled(false);
    connect(ui->chkFilter, &QCheckBox::toggled, this, &ReturnDialog::enableFilter);
    connect(ui->lnFilter, &QLineEdit::textEdited, this, &ReturnDialog::filter);
}

ReturnDialog::~ReturnDialog()
{
    save();
    qDeleteAll(mControls);
    mControls.clear();

    delete ui;
}


void ReturnDialog::keyPressEvent(QKeyEvent *k)
{
    ui->barCode->setFocus();
}

void ReturnDialog::selectBarCode()
{
    ui->barCode->setFocus();
}


void ReturnDialog::barCodeEntered()
{
    mParser.parse(ui->barCode->text());

    if(mParser.type() == BarCodeParser::Types::Grouped || mParser.type() == BarCodeParser::Types::Unitary)
    {
        InventoryAction *action = new InventoryAction(mParser.item(), 1);

        if(mParser.hasPendingCommand())
        {
            auto command = mParser.command();

            switch(command.first)
            {
                case BarCodeParser::Add:
                    action->qty = command.second;
                    break;

                case BarCodeParser::Remove:
                    action->qty = -command.second;
                    break;

                case BarCodeParser::None:
                break;
            }

        }

        ui->barCodeCommand->setText(mInventory.item(mParser.item()).name() + "\n" + mInventory.item(mParser.item()).reference());
        inventoryChange(action);
    }

    else if(mParser.type() == BarCodeParser::Types::Commands)
        ui->barCodeCommand->setText(mParser.textCommand());

    else
        ui->barCodeCommand->setText("");



    ui->barCode->clear();
}


void ReturnDialog::undo()
{

}

void ReturnDialog::redo()
{

}


void ReturnDialog::archiveAll()
{
    for(auto &m : mControls)
    {
        if(!m->isArchived())
            m->setArchived(true);
    }
}

void ReturnDialog::unarchiveAll()
{
    for(auto &m : mControls)
    {
        if(m->isArchived())
            m->setArchived(false);
    }
}

void ReturnDialog::inventoryChange(InventoryAction *action)
{
    mAutoSave.start();
    mInventory.perform(action);
}

void ReturnDialog::save()
{
    // API put all data
    mAutoSave.stop();

    api->returnMaterial(mEvent, mInventory);
}


void ReturnDialog::enableFilter(bool state)
{
    if(!state)
    {
        for(auto &control : mControls)
        {
            if(!control->isArchived())
                control->show();
        }

        ui->lnFilter->setEnabled(false);
    }

    else
    {
        ui->lnFilter->setEnabled(true);
        filter(ui->lnFilter->text());
    }
}

void ReturnDialog::filter(QString filter)
{
    for(auto &control : mControls)
    {
        if(!control->line.name().contains(filter, Qt::CaseSensitivity::CaseInsensitive))
            control->hide();
        else if(!control->isArchived())
            control->show();
    }
}

