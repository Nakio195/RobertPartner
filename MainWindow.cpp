#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "widgets/ReturnDialog.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    delayedFetch.setInterval(1000);
    connect(&delayedFetch, &QTimer::timeout, this, &MainWindow::fetchEvents);

    ui->setupUi(this);


    api = new RestAccessManager("http://localhost", 80, ui->apiLog);
    events = new EventModel(api);


    mAuthDialog = new AuthDialog(api, this);

    qDebug() << mAuthDialog->exec();

    ui->apiLog->setVisible(false);
    ui->progressBar->hide();

    ui->datStart->setDate(QDate::currentDate().addDays(-15));
    ui->datEnd->setDate(QDate::currentDate().addDays(15));

    ui->tblEvent->setModel(events);
    ui->tblEvent->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    events->fetch(ui->datStart->date(), ui->datEnd->date());

    connect(api, &RestAccessManager::requestStarted, ui->progressBar, &QProgressBar::show);
    connect(api, &RestAccessManager::requestFinished, ui->progressBar, &QProgressBar::hide);
    connect(ui->tblEvent->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &MainWindow::selectionChanged);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::fetchEvents()
{
    delayedFetch.stop();
    events->fetch(ui->datStart->date(), ui->datEnd->date());
}

void MainWindow::startDateChanged()
{
    QDate start = ui->datStart->date();
    QDate end = ui->datEnd->date();

    if(start >= end)
        ui->datStart->setDate(end.addDays(-1));

    if(end.toJulianDay() - start.toJulianDay() >= 100)
        ui->datEnd->setDate(start.addDays(100));

    delayedFetch.start();
}

void MainWindow::endDateChanged()
{
    QDate start = ui->datStart->date();
    QDate end = ui->datEnd->date();

    if(end <= start)
        ui->datStart->setDate(end.addDays(-1));

    if(end.toJulianDay() - start.toJulianDay() >= 100)
        ui->datStart->setDate(end.addDays(-100));

    delayedFetch.start();
}


void MainWindow::centerOnToday()
{
    ui->datStart->setDate(QDate::currentDate().addDays(-15));
    ui->datEnd->setDate(QDate::currentDate().addDays(15));

    delayedFetch.start();
}

void MainWindow::selectionChanged(const QModelIndex &index)
{
    activeEvent = events->getEvent(index);

    ui->btnReturn->setEnabled(!activeEvent.returned);
    ui->btnDeparture->setEnabled(!activeEvent.departed);
}

void MainWindow::startDeparture()
{
    ReturnDialog departure(events->getEvent(ui->tblEvent->currentIndex()), api);
    departure.exec();
}
