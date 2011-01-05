#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "recoverthread.h"
#include <QDebug>

static RecoverThread* recoverThread = NULL;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionRecover_triggered()
{
    qDebug() << "on_actionRecover_triggered";
    if (!recoverThread) {
        recoverThread = new RecoverThread();
        connect(recoverThread, SIGNAL(finished()), this, SLOT(on_recoverThread_finished()));
        recoverThread->start();
    }
}

void MainWindow::on_actionAbort_triggered()
{
    recoverThread->abort();
}

void MainWindow::on_recoverThread_finished(){
    recoverThread->disconnect();
    delete recoverThread;
    recoverThread = NULL;
}
