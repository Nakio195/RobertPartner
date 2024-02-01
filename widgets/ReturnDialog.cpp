#include "ReturnDialog.h"
#include "ui_ReturnDialog.h"
#include <QAction>

ReturnDialog::ReturnDialog(Event event, RestAccessManager *api, QWidget *parent) : QDialog(parent), ui(new Ui::ReturnDialog), api(api), event(event)
{
    ui->setupUi(this);

    event = Event(api->event(event.id));

    for(auto m : event.materials)
    {
        MaterialLine *line = new MaterialLine(ui->listLayout, m, m.qty, m.qtyDeparted, m.qtyReturned, m.qtyFault);

        connect(line, &MaterialLine::archive, this, &ReturnDialog::archive);
        connect(line, &MaterialLine::qtyChange, this, &ReturnDialog::inventoryChange);

        list.insert(m.id, line);

    }

    ui->eventName->setText(event.name);

    autoSave.setInterval(3000);
    connect(&autoSave, &QTimer::timeout, this, &ReturnDialog::save);

    barCodeShortcut = new QShortcut(this);
    barCodeShortcut->setKey(QKeySequence("Ctrl+^"));
    connect(barCodeShortcut, &QShortcut::activated, this, &ReturnDialog::selectBarCode);

}

ReturnDialog::~ReturnDialog()
{
    save();
    qDeleteAll(list);
    list.clear();

    delete ui;
}


void ReturnDialog::keyPressEvent(QKeyEvent *k)
{
    if(k->key() == 30)
        ui->barCode->setFocus();

    qDebug() << k->key();
}

void ReturnDialog::selectBarCode()
{
    ui->barCode->setFocus();
}

void ReturnDialog::inventoryChange()
{
    autoSave.start();
}

void ReturnDialog::save()
{
    // API put all data
    autoSave.stop();

    QList<Material> changedLines;

    for(const auto mat : std::as_const(list))
    {
        changedLines.append(mat->getMaterial());
    }

    if(!changedLines.empty())
        api->returnMaterial(event, changedLines);
}

void ReturnDialog::archive(const Material &mat)
{
    if(list.contains(mat.id))
    {
        list[mat.id]->hide();
        //API Put to Return inventory
    }
}
