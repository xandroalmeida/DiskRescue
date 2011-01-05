#ifndef RECOVERTHREAD_H
#define RECOVERTHREAD_H

#include <QThread>

class RecoverThread : public QThread
{
    Q_OBJECT
private:
    bool m_abort;
public:
    RecoverThread();
    virtual void run();

public slots:
    void abort();
};

#endif // RECOVERTHREAD_H
