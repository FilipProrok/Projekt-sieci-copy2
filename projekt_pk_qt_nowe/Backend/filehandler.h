#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <QString>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>

class FileHandler
{
public:
    FileHandler();

    bool save(const QJsonObject &data, const QString &fileName);
    QJsonObject load(const QString &fileName);
};

#endif // FILEHANDLER_H
