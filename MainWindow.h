#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include "SEB/RESTManager.h"
#include "widgets/AuthDialog.h"
#include "SEB/EventModel.h"
#include "widgets/DepartureDialog.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    private:
        RestAccessManager *api;
        EventModel *events;

    public slots:
        void startDateChanged();
        void endDateChanged();
        void centerOnToday();

        void fetchEvents();
        void startDeparture();

    private:
        Ui::MainWindow *ui;
        AuthDialog* mAuthDialog;

        QTimer delayedFetch;


};
#endif // MAINWINDOW_H
