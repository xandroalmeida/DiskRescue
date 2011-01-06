#ifndef RECOVERTHREAD_H
#define RECOVERTHREAD_H

#include "ddrescuestatus.h"
#include "ddrescuelog.h"

#include <QThread>

class RecoverThread : public QThread
{
    Q_OBJECT
private:
    bool m_abort;
    QString m_outputDir;
    QString m_diskName;
public:
    RecoverThread(QString const &outputDir, QString const &diskName);
    virtual void run();

public slots:
    void abort();

signals:
    void updateStatus(DdrescueStatus const &status, DdrescueLog const &log);

};

#endif // RECOVERTHREAD_H
