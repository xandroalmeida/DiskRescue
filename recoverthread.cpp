#include "recoverthread.h"
#include <QProcess>
#include <QRegExp>
#include <QDebug>

RecoverThread::RecoverThread():
        m_abort(false)
{
}

void RecoverThread::run() {

    QRegExp re("\\W*rescued:\\s+(\\d+)\\W+(\\w+B)(.*)\\.\\.\\.");
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
            proc.kill();
            proc.waitForFinished();
            break;
        }

        QString out = proc.readAll();
        if (out.size() > 0 &&  re.indexIn(out) > 0) {
            qDebug() << "--->" << out;
            qDebug() << re.cap(1) << re.cap(2) << re.cap(3);
        }
    }

    qDebug() << "finished" << proc.readAll();
}

void RecoverThread::abort() {
    m_abort = true;
}
