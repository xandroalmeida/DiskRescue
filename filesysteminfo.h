#ifndef FILESYSTEMINFO_H
#define FILESYSTEMINFO_H

#include <QList>
#include <QString>
#include <QMetaType>

class FileSystemInfo
{
public:
    FileSystemInfo();
    FileSystemInfo(int type, QString fsname, QString devname, QString vlname);
    static QList<FileSystemInfo> getFilesSystemList();

    int getType();
    QString getFsname();
    QString getDevname();
    QString getVolname();

private:
    int m_type;
    QString m_fsname;
    QString m_devname;
    QString m_vlname;
};

Q_DECLARE_METATYPE(FileSystemInfo)

#endif // FILESYSTEMINFO_H
