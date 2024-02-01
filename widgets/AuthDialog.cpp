#include "AuthDialog.h"
#include "ui_AuthDialog.h"

AuthDialog::AuthDialog(RestAccessManager *API, QWidget *parent) : QDialog(parent), ui(new Ui::AuthDialog)
{
    ui->setupUi(this);

    ui->progressBar->hide();

    if(API == nullptr)
        return;

    api = API;

    connect(api, &RestAccessManager::authSuccess, this, &AuthDialog::authSuccess);
}

AuthDialog::~AuthDialog()
{
    delete ui;
}


void AuthDialog::submit()
{
    api->setUrl(ui->txtUrl->text());
    api->submitCredentials(ui->txtIdentifier->text(), ui->txtPassword->text());

    ui->progressBar->show();
}


void AuthDialog::authSuccess(bool isSuccess)
{
    ui->progressBar->hide();

    if(isSuccess)
        this->accept();

    else
        ui->lblStatus->setText("Erreur d'authentification !");
}
