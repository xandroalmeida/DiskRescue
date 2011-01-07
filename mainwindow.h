#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ddrescuestatus.h"
#include "ddrescuelog.h"
#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;    
    void checkGuiStatus();
    bool m_closeOnFinished;
protected:
    virtual void closeEvent(QCloseEvent *);

private slots:
    void on_cbCdDriver_currentIndexChanged(QString const &);
    void on_btnSelectOutputDirectory_clicked();
    void on_actionAbort_triggered();
    void on_actionRecover_triggered();
    void on_recoverThread_finished();

    void on_recoverThread_updateStatus(DdrescueStatus const & status, DdrescueLog const &log);

};

#endif // MAINWINDOW_H
