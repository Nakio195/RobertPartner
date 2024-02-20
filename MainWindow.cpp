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
    ui->tblEvent->horizontalHeader()->setStretchLastSection(true);
    ui->tblEvent->setColumnWidth(1, 450);
    ui->tblEvent->hideColumn(0);
    ui->tblEvent->hideColumn(4);
    ui->tblEvent->hideColumn(5);

    events->fetch(ui->datStart->date(), ui->datEnd->date());

    connect(api, &RestAccessManager::requestStateChanged, ui->progressBar, &QProgressBar::setVisible);
    connect(api, &RestAccessManager::dataValidityChanged, ui->tblEvent, &QTableView::setEnabled);
    connect(ui->tblEvent->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &MainWindow::selectionChanged);
    connect(ui->datStart, &QDateEdit::dateChanged, this, &MainWindow::startDateChanged);
    connect(ui->datEnd, &QDateEdit::dateChanged, this, &MainWindow::endDateChanged);
    connect(ui->chkDates, &QCheckBox::stateChanged, this, &MainWindow::setDatesVisibility);

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

void MainWindow::setDatesVisibility(bool visibility)
{
    ui->tblEvent->setColumnHidden(4, !visibility);
    ui->tblEvent->setColumnHidden(5, !visibility);
}

void MainWindow::selectionChanged(const QModelIndex &index)
{
    activeEvent = events->getEvent(index);

    ui->btnReturn->setEnabled(!activeEvent.returned && activeEvent.start <= QDate::currentDate());
    ui->btnDeparture->setEnabled(!activeEvent.departed && activeEvent.end >= QDate::currentDate());
}

void MainWindow::startDeparture()
{
    ReturnDialog departure(events->getEvent(ui->tblEvent->currentIndex()), api);
    departure.exec();
}
