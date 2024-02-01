#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    delayedFetch.setInterval(1000);
    connect(&delayedFetch, &QTimer::timeout, this, &MainWindow::fetchEvents);

    api = new RestAccessManager("http://localhost", 80);
    events = new EventModel(api);

    ui->setupUi(this);

    mAuthDialog = new AuthDialog(api, this);

    qDebug() << mAuthDialog->exec();

    ui->progressBar->hide();

    ui->datStart->setDate(QDate::currentDate().addDays(-15));
    ui->datEnd->setDate(QDate::currentDate().addDays(15));

    ui->tblEvent->setModel(events);
    ui->tblEvent->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    events->fetch(ui->datStart->date(), ui->datEnd->date());

    connect(api, &RestAccessManager::requestStarted, ui->progressBar, &QProgressBar::show);
    connect(api, &RestAccessManager::requestFinished, ui->progressBar, &QProgressBar::hide);

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

void MainWindow::startDeparture()
{
    DepartureDialog departure(events->getEvent(ui->tblEvent->currentIndex()), api);
    departure.exec();
}
