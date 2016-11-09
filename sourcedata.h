#ifndef SOURCEDATA_H
#define SOURCEDATA_H

#include <vector>
#include <QPolygonF>
#include <QJsonDocument>
#include "configurationspace.h"

class SourceData : public std::vector<ConfigurationSpace>
{
public:
    SourceData(const QString& path);
};

#endif // SOURCEDATA_H
