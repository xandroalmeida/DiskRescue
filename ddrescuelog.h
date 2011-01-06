#ifndef DDRESCUELOG_H
#define DDRESCUELOG_H

#include <QString>
#include <QMetaType>
#include <QVariant>
#include <QList>

class DdrescueLog
{
    class BlockInfo{
    public:
        long long pos;
        long long size;
        QChar status;
    };

private:
    long m_currentPos;
    QChar m_currentStatus;
    QList<BlockInfo> m_blocks;

public:
    DdrescueLog();
    bool load(QString const & fileName);

    long long currentPos() const;
    QChar currentStatus() const;
    QList<BlockInfo> blocks() const;

};

Q_DECLARE_METATYPE(DdrescueLog)

#endif // DDRESCUELOG_H
