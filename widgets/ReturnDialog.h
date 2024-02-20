#ifndef RETURNDIALOG_H
#define RETURNDIALOG_H

#include <QDialog>
#include <QTimer>
#include <QShortcut>
#include <QKeyEvent>
#include <QStack>

#include "widgets/MaterialLine.h"

#include "SEB/Item.h"
#include "SEB/Inventory.h"
#include "SEB/RESTManager.h"
#include "SEB/Event.h"
#include "SEB/InventoryAction.h"
#include "SEB/BarCodeParser.h"

namespace Ui {
class ReturnDialog;
}


class ReturnDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit ReturnDialog(Event event, RestAccessManager *api,  QWidget *parent = nullptr);
        ~ReturnDialog();

    public slots:
        void enableFilter(bool state);
        void filter(QString filter);

    private slots:
        void save();

        void inventoryChange(InventoryAction *action);

        void undo();
        void redo();

        void selectBarCode();
        void barCodeEntered();

        void archiveAll();
        void unarchiveAll();

        void keyPressEvent(QKeyEvent *k) override;

    private:
        Ui::ReturnDialog *ui;
        RestAccessManager *api;

        Event mEvent;
        Inventory mInventory;

        QMap<ItemID, MaterialLine*> mControls;
        BarCodeParser mParser;


        QTimer mAutoSave;

        QShortcut *barCodeShortcut;
};



#endif // RETURNDIALOG_H
