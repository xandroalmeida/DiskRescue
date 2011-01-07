#ifndef RECOVERTHREAD_H
#define RECOVERTHREAD_H

#include "ddrescuestatus.h"
#include "ddrescuelog.h"

#include <QThread>
#include <QDateTime>

class RecoverThread : public QThread
{
    Q_OBJECT
private:
    bool m_abort;
    QString m_outputDir;
    QString m_diskName;
    bool m_aggressive;
    QDateTime m_dtStart;
public:
    RecoverThread(QString const &outputDir, QString const &diskName, bool aggressive);
    virtual void run();
    QDateTime dtStart() const {return m_dtStart;};

public slots:
    void abort();

signals:
    void updateStatus(DdrescueStatus const &status, DdrescueLog const &log);

};

#endif // RECOVERTHREAD_H
