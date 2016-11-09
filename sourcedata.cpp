#include "sourcedata.h"
#include <QDir>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>
#include <QtDebug>

SourceData::SourceData(const QString &path)
{
    QDir dir(path);
    dir.setFilter(QDir::Files);
    dir.setNameFilters(QStringList() << "*.json");
    this->reserve(dir.count());
    for (const QFileInfo& file : dir.entryInfoList()) {
        QFile f(file.absoluteFilePath());
        qInfo() << "Finded data" << file.fileName().split('.').first();
        f.open(QFile::ReadOnly);
        this->emplace_back(QJsonDocument::fromJson(f.readAll()).object(), file.fileName().split('.').first());
    }
}
