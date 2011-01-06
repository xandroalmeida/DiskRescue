#include "ddrescuestatus.h"
#include <QStringList>
#include <QRegExp>
#include <QDebug>

static long long calcByteQuantity(QString const & value, QString const & unit)
{
    long long lValue = value.toLongLong();
    if (unit == "B") {
        return lValue;
    } else if (unit == "kB") {
        return lValue * 1e3;
    } else if (unit == "MB") {
        return lValue * 1e6;
    } else if (unit == "GB") {
        return lValue * 1e9;
    } else if (unit == "TB") {
        return lValue * 1e12;
    } else if (unit == "PB") {
        return lValue * 1e15;
    } else if (unit == "EB") {
        return lValue * 1e18;
    }

    return -1;
}

static long long calcByteRate(QString const & value, QString const & unit)
{
    QString u = unit;
    return calcByteQuantity(value, u.remove("/s"));
}

static long calcTimeQuantity(QString const & value, QString const & unit)
{
    return value.toLong();
}

DdrescueStatus::DdrescueStatus() :
        m_parseOk(false),
        m_rescued(0),
        m_errsize(0),
        m_currentRate(0),
        m_ipos(0),
        m_errors(0),
        m_averageRate(0),
        m_opos(0),
        m_timeFromLastSuccessfulRead(0),
        m_status("")
{
}

DdrescueStatus::DdrescueStatus(QString const & txt)
{
    parse(txt);
}

bool DdrescueStatus::parse(QString const & txt)
{
    int idx = -1;
    QString log = txt;
    if (txt.size() <= 0 ||&  (idx = txt.lastIndexOf("rescued")) < 0)
        return false;
    log = txt.mid(idx);

    QStringList fields = log.split(QRegExp("[,\\n]"));

    for (int i = 0; i < fields.size()-1; i++) {
        QStringList keyvalue = fields.at(i).trimmed().split(":");
        QString key = keyvalue.at(0).trimmed();
        QString value = keyvalue.at(1).trimmed();
        QRegExp re("(\\d+)(.*)?");
        if (re.indexIn(value) >= 0) {
            value = re.cap(1);
            QString unit = re.cap(2).trimmed();
            if (key == "rescued")
                m_rescued = calcByteQuantity(value, unit);
            else if (key == "errsize")
                m_errsize = calcByteQuantity(value, unit);
            else if (key == "current rate")
                m_currentRate = calcByteRate(value, unit);
            else if (key == "ipos")
                m_ipos = calcByteQuantity(value, unit);
            else if (key == "opos")
                m_opos = calcByteQuantity(value, unit);
            else if (key == "errors")
                m_errors = value.toLong();
            else if (key == "average rate")
                m_averageRate = calcByteRate(value, unit);
            else if (key == "time from last successful read")
                m_timeFromLastSuccessfulRead = calcTimeQuantity(value, unit);
        }

    }

    m_status = fields.at(fields.size()-1);

    return true;
}

bool DdrescueStatus::parseOk() const
{
    return m_parseOk;
}

long long DdrescueStatus::rescued() const
{
    return m_rescued;
}

long long DdrescueStatus::errsize() const
{
    return m_errsize;
}

long DdrescueStatus::currentRate() const
{
    return m_currentRate;
}

long long DdrescueStatus::ipos() const
{
    return m_ipos;
}

long DdrescueStatus::errors() const
{
    return m_errors;
}

long DdrescueStatus::averageRate() const
{
    return m_averageRate;
}

long long DdrescueStatus::opos() const
{
    return m_opos;
}

int DdrescueStatus::timeFromLastSuccessfulRead() const
{
    return m_timeFromLastSuccessfulRead;
}

QString DdrescueStatus::status() const
{
    return m_status;
}
