#include "recoverthread.h"
#include <QProcess>
#include <QDebug>

RecoverThread::RecoverThread():
        m_abort(false)
{
}

void RecoverThread::run() {
    QProcess proc;
    QString prg = "C:\\Projetos\\sonitIT\\DiskRescue-build-desktop\\debug\\ddrescue.exe";
    QStringList args = QStringList() << "-n" << "-b2048" << "/dev/sr0" << "c:\\Alexandro\\rescue.iso" << "c:\\Alexandro\\rescue.log";
    qDebug() << " RecoverThread::run";

    proc.start(prg, args, QProcess::ReadOnly);
    if (!proc.waitForStarted()) {
        qDebug() << "Not stared";
        return;
    }

    while (!proc.waitForFinished(500)) {
        if (m_abort) {
            proc.close();
            proc.waitForFinished();
            break;
        }

        qDebug() << proc.readAll();
    }

    qDebug() << "finished" << proc.readAll();
}

void RecoverThread::abort() {
    m_abort = true;
}
