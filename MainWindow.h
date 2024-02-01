#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include "SEB/RESTManager.h"
#include "widgets/AuthDialog.h"
#include "SEB/EventModel.h"

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


    public slots:
        void startDateChanged();
        void endDateChanged();
        void centerOnToday();
        void selectionChanged(const QModelIndex &index);

        void fetchEvents();
        void startDeparture();

    private:
        void eventInformations();

    private:
        RestAccessManager *api;
        EventModel *events;

        Ui::MainWindow *ui;
        AuthDialog* mAuthDialog;

        QTimer delayedFetch;

        Event activeEvent;


};
#endif // MAINWINDOW_H
