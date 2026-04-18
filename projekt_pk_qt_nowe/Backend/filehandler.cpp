#include "filehandler.h"

FileHandler::FileHandler() {}

bool FileHandler::save(const QJsonObject &data, const QString &fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) return false;

    QJsonDocument doc(data);
    file.write(doc.toJson());
    file.close();
    return true;
}

QJsonObject FileHandler::load(const QString &fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) return QJsonObject();

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    return doc.object();
}
