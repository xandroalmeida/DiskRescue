#include "filesysteminfo.h"

#include <QDebug>
#include "windows.h"

FileSystemInfo::FileSystemInfo()
{
    this->m_type = -1;
    this->m_fsname = "";
    this->m_devname = "";
    this->m_vlname = "";
}

FileSystemInfo::FileSystemInfo(int type, QString fsname, QString devname, QString vlname)
{
    this->m_type = type;
    this->m_fsname = fsname;
    this->m_devname = devname;
    this->m_vlname = vlname;

}

QList<FileSystemInfo> FileSystemInfo::getFilesSystemList()
{
    QList<FileSystemInfo> list;

    char name[256];
    char *ptr = name;

    DWORD len = GetLogicalDriveStringsA(sizeof(name), name);
    qDebug() << "GetLogicalDriveStringsA len:" << len;

    if (len > 0) {

        while (*ptr) {
            qDebug() << "GetLogicalDriveStringsA name:" << ptr;

            DWORD flags, serialnum=0;
            char fsname[1024];
            char vlname[1024];
            UINT type = GetDriveTypeA(ptr);

            fsname[0] = '\0';
            vlname[0] = '\0';

            GetVolumeInformationA(ptr, vlname, sizeof(vlname), &serialnum, NULL, &flags, fsname, sizeof(fsname));

            list.append(FileSystemInfo(type, QString(fsname), QString(ptr), QString(vlname)));
            qDebug() << "GetLogicalDriveStringsA type:" << type;
            qDebug() << "GetLogicalDriveStringsA serialnum:" << serialnum;
            qDebug() << "GetLogicalDriveStringsA fsname:" << fsname;
            qDebug() << "GetLogicalDriveStringsA vlname:" << vlname;

            ptr += strlen(ptr)+1;
        }
    }
    return list;
}

int FileSystemInfo::getType()
{
    return this->m_type;
}

QString FileSystemInfo::getFsname()
{
    return this->m_fsname;
}

QString FileSystemInfo::getVolname()
{
    return this->m_vlname;
}

QString FileSystemInfo::getDevname()
{
    return this->m_devname;
}
