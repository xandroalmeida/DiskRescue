#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "recoverthread.h"
#include <QDebug>
#include <QSettings>
#include <QFileDialog>
#include <QGraphicsScene>
#include <qt_windows.h>

static RecoverThread* recoverThread = NULL;

static QColor statusBlock2QColor(QChar status) {
    if (status == '?')     //non-tried block
        return QColor(Qt::gray);

    if (status == '*')     //failed block non-trimmed
        return QColor(Qt::yellow);

    if (status == '/')     //failed block non-split
        return QColor(Qt::magenta);

    if (status == '-')     //failed block bad-sector(s)
        return QColor(Qt::red);

    if (status == '+')     //finished block
        return QColor(Qt::green);

    return QColor(Qt::cyan);

}


static QString TvInfo(QString const & tdrive)
{
    WCHAR szVolumeName[256] ;
    WCHAR szFileSystemName[256];
    DWORD dwSerialNumber = 0;
    DWORD dwMaxFileNameLength=256;
    DWORD dwFileSystemFlags=0;

    bool ret = GetVolumeInformation( (WCHAR *) tdrive.utf16(),szVolumeName,256,&dwSerialNumber,&dwMaxFileNameLength,&dwFileSystemFlags,szFileSystemName,256);
    if(!ret)
        return QString("");

    QString vName=QString::fromUtf16 ( (const ushort *) szVolumeName) ;
    vName.trimmed();
    return vName;
}

static QString formatBytes(long long value)
{
    if (value > 2*1e9)
        return QString::number(value/1e9) + " GB";
    if (value > 2*1e6)
        return QString::number(value/1e6) + " MB";
    if (value > 2*1e3)
        return QString::number(value/1e3) + " kB";

    return QString::number(value) + " B";
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{ 
    ui->setupUi(this);
    ui->txtOutputDirectory->setText(QSettings().value("outputDirectory").value<QString>());

    checkGuiStatus();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionRecover_triggered()
{
    qDebug() << "on_actionRecover_triggered";
    if (!recoverThread) {
        recoverThread = new RecoverThread(ui->txtOutputDirectory->text(), ui->lblVolumeName->text());
        connect(recoverThread, SIGNAL(finished()), this, SLOT(on_recoverThread_finished()));
        connect(recoverThread, SIGNAL(updateStatus(DdrescueStatus, DdrescueLog)), this, SLOT(on_recoverThread_updateStatus(DdrescueStatus, DdrescueLog)));

        recoverThread->start();
        checkGuiStatus();
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
    checkGuiStatus();
}

void MainWindow::checkGuiStatus()
{
    if (recoverThread) {
        ui->btnSelectOutputDirectory->setEnabled(false);
        ui->actionRecover->setEnabled(false);
        ui->actionAbort->setEnabled(true);
        ui->cbCdDriver->setEnabled(false);
    } else {
        ui->btnSelectOutputDirectory->setEnabled(true);
        ui->actionRecover->setEnabled(ui->lblVolumeName->text().size() > 0);
        ui->actionAbort->setEnabled(false);
        ui->cbCdDriver->setEnabled(true);
    }
}

void MainWindow::on_recoverThread_updateStatus(const DdrescueStatus &status,  DdrescueLog const &log)
{
    ui->lblRescuedValue->setText(formatBytes(status.rescued()));
    ui->lblIposValue->setText(formatBytes(status.ipos()));
    ui->lblOposValue->setText(formatBytes(status.opos()));

    ui->lblErrsizeValue->setText(formatBytes(status.errsize()));
    ui->lblErrorsValue->setText(QString::number(status.errors()));

    ui->lblAvgRateValue->setText(formatBytes(status.averageRate()) + "/s");
    ui->lblCurrRateValue->setText(formatBytes(status.currentRate()) + "/s");

    ui->lblTimeFrom->setText(QString::number(status.timeFromLastSuccessfulRead()) + " s");

    ui->lblStatus->setText(status.status());

    QGraphicsScene *scene = ui->graph->scene();
    if (!scene) {
        scene = new QGraphicsScene();
        ui->graph->setScene(scene);
    }
    scene->clear();
    qreal width = ui->graph->width()-5;
    qreal height = ui->graph->height()-5;
    scene->addRect(0,0,width,height,QPen(QColor(Qt::blue)), QBrush(QColor(Qt::blue)));

    long long sizeTotal = 0;
    for (int i = 0; i < log.blocks().size(); i++) {
        sizeTotal += log.blocks().at(i).size;
    }
    qreal wfactor = sizeTotal / width;


    if (sizeTotal > 0) {
       for (int i = 0; i < log.blocks().size(); i++) {
           qreal x = log.blocks().at(i).pos/wfactor;
           qreal w = log.blocks().at(i).size/wfactor;

            scene->addRect(x,0
                         ,w,height
                         ,QPen(statusBlock2QColor(log.blocks().at(i).status))
                         , QBrush(statusBlock2QColor(log.blocks().at(i).status)));
        }
    }

    scene->addText(formatBytes(sizeTotal));
}

void MainWindow::on_btnSelectOutputDirectory_clicked()
{
    QFileDialog fileDlg;
    fileDlg.setOption(QFileDialog::ShowDirsOnly, true);
    fileDlg.setFileMode(QFileDialog::DirectoryOnly);
    fileDlg.setDirectory(ui->txtOutputDirectory->text());
    if (fileDlg.exec())
    {
        ui->txtOutputDirectory->setText(fileDlg.selectedFiles().at(0));
        QSettings().setValue("outputDirectory", ui->txtOutputDirectory->text());
    }
}

void MainWindow::on_cbCdDriver_currentIndexChanged(QString const &driverName)
{
    ui->lblVolumeName->setText(TvInfo(driverName));
    checkGuiStatus();
}
