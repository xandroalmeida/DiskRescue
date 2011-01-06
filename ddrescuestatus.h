#ifndef DDRESCUESTATUS_H
#define DDRESCUESTATUS_H
#include <QString>
#include <QMetaType>
#include <QVariant>

class DdrescueStatus
{
private:
    bool m_parseOk;
    long m_rescued;
    long m_errsize;
    long m_currentRate;
    long m_ipos;
    long m_errors;
    long m_averageRate;
    long m_opos;
    int m_timeFromLastSuccessfulRead;

    QString m_status;

public:
    DdrescueStatus();
    DdrescueStatus(QString const & txt);
    bool parse(QString const & txt);

    bool parseOk() const;
    long rescued() const;
    long errsize() const;
    long currentRate() const;
    long ipos() const;
    long errors() const;
    long averageRate() const;
    long opos() const;
    int timeFromLastSuccessfulRead() const;
    QString status() const;

};
Q_DECLARE_METATYPE(DdrescueStatus)
#endif // DDRESCUESTATUS_H
