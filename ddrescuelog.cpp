#include "ddrescuelog.h"
#include <QFile>
#include <QDir>
#include <QRegExp>
#include <QDebug>



DdrescueLog::DdrescueLog():
        m_currentPos(-1),
        m_currentStatus()
{
}

bool DdrescueLog::load(QString const & fileName)
{
    QFile logFile(fileName);
    if (logFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        m_blocks.clear();
        while (!logFile.atEnd()) {
            QString line(logFile.readLine());
            if (line.startsWith("#"))
                continue;
            QRegExp re ("^(0x[0-9a-fA-F]+)\\s+(.)\n$");
            if (re.indexIn(line) >= 0) {
                m_currentPos = re.cap(1).toLong(NULL, 16);
                m_currentStatus = re.cap(2).at(0);

            }
            re.setPattern("^(0x[0-9a-fA-F]+)\\s+(0x[0-9a-fA-F]+)\\s+(.)\n$");
            if (re.indexIn(line) >= 0) {
                BlockInfo bi;
                bi.pos = re.cap(1).toLong(NULL, 16);
                bi.size = re.cap(2).toLong(NULL, 16);
                bi.status = re.cap(3).at(0);
                m_blocks.append(bi);
            }
            //qDebug() << line;
        }
    } else {
        qDebug() << "Erro ao abrir " << fileName;
    }


}

long DdrescueLog::currentPos() const
{
    return m_currentPos;
}


QChar DdrescueLog::currentStatus() const
{
    return m_currentStatus;
}
