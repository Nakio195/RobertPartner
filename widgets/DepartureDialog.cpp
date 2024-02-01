#include "DepartureDialog.h"
#include "ui_DepartureDialog.h"

DepartureDialog::DepartureDialog(Event event, RestAccessManager *api, QWidget *parent) : QDialog(parent), ui(new Ui::DepartureDialog), api(api), event(event)
{
    ui->setupUi(this);

    event = Event(api->event(event.id));

    for(auto m : event.materials)
    {
        list.insert(m, new MaterialLine(ui->listLayout, m, m.qty, m.qtyDeparted, m.qtyReturned, m.qtyFault));
    }

    ui->eventName->setText(event.name);
}

DepartureDialog::~DepartureDialog()
{
    qDeleteAll(list);
    list.clear();

    delete ui;
}
