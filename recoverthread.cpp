#include "recoverthread.h"
#include "ddrescuelog.h"

#include <QProcess>
#include <QRegExp>
#include <QDebug>
#include <QSettings>
#include <QtGui/QApplication>

RecoverThread::RecoverThread(QString const &outputDir, QString const &diskName):
        m_abort(false)
{
    m_outputDir = outputDir;
    m_diskName = diskName;
}

void RecoverThread::run() {

#ifndef QT_NO_DEBUG
    QString prg = "C:\\Projetos\\sonitIT\\DiskRescue-build-desktop\\debug\\ddrescue.exe";
    ;
#else
    QSettings settings("HKEY_CURRENT_USER\\Software\\" + QApplication::organizationName() + "\\" + QApplication::applicationName(), QSettings::NativeFormat);

#endif

    QProcess proc;
    QStringList args = QStringList() << "-n" << "-b2048" << "/dev/sr0" << m_outputDir + "\\" + m_diskName + ".iso" << m_outputDir + "\\" + m_diskName + ".log";
    qDebug() << " RecoverThread::run";

    proc.start(prg, args, QProcess::ReadOnly);
    if (!proc.waitForStarted()) {
        qDebug() << "Not stared";
        return exit(-1);
    }

    DdrescueStatus status;
    while (!proc.waitForFinished(500)) {
        if (m_abort) {
            proc.kill();
            proc.waitForFinished();
            break;
        }

        QString out = proc.readAll();
        if (status.parse(out)) {
            DdrescueLog log;
            log.load( m_outputDir + "\\" + m_diskName + ".log");
            emit(updateStatus(status, log));
        }
    }

    qDebug() << "finished" << proc.readAll();
}

void RecoverThread::abort() {
    m_abort = true;
}
