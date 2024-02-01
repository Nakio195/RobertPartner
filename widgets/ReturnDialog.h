#ifndef RETURNDIALOG_H
#define RETURNDIALOG_H

#include <QDialog>
#include <QTimer>
#include <QShortcut>
#include <QKeyEvent>

#include "SEB/Material.h"
#include "widgets/MaterialLine.h"
#include "SEB/RESTManager.h"
#include "SEB/Event.h"

namespace Ui {
class ReturnDialog;
}

class ReturnDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit ReturnDialog(Event event, RestAccessManager *api,  QWidget *parent = nullptr);
        ~ReturnDialog();

    private slots:
        void archive(const Material &mat);
        void inventoryChange();
        void save();

        void selectBarCode();

        void keyPressEvent(QKeyEvent *k) override;

    private:
        Ui::ReturnDialog *ui;
        RestAccessManager *api;

        Event event;
        QMap<int, MaterialLine*> list;

        QTimer autoSave;

        QShortcut *barCodeShortcut;
};

#endif // RETURNDIALOG_H
