#ifndef DEPARTUREDIALOG_H
#define DEPARTUREDIALOG_H

#include <QDialog>
#include "SEB/Material.h"
#include "widgets/MaterialLine.h"
#include "SEB/RESTManager.h"
#include "SEB/Event.h"

namespace Ui {
class DepartureDialog;
}

class DepartureDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit DepartureDialog(Event event, RestAccessManager *api,  QWidget *parent = nullptr);
        ~DepartureDialog();

    private slots:

    private:
        Ui::DepartureDialog *ui;

        RestAccessManager *api;

        Event event;
        QMap<Material, MaterialLine*> list;
};

#endif // DEPARTUREDIALOG_H
