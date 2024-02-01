#ifndef AUTHDIALOG_H
#define AUTHDIALOG_H

#include <QDialog>
#include "../SEB/RESTManager.h"

namespace Ui {
class AuthDialog;
}

class AuthDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit AuthDialog(RestAccessManager *API, QWidget *parent = nullptr);
        ~AuthDialog();


    public slots:
        void submit();
        void authSuccess(bool isSuccess);

    private:
        Ui::AuthDialog *ui;

        RestAccessManager* api;
};

#endif // AUTHDIALOG_H
