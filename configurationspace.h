#ifndef CONFIGURATIONSPACE_H
#define CONFIGURATIONSPACE_H

#include <QPoint>
#include <vector>
#include "polygon.h"

class QJsonObject;

class ConfigurationSpace
{
public:
    ConfigurationSpace(const QJsonObject&, const QString&);
    const QString& getName() const { return name; }

    bool isValid(const QPointF&) const;
    QSizeF spaceSize() const { return SIZE; }
    bool isApparent(const QPointF&, const QPointF&) const;

    const std::vector<Polygon>& getObstacles() const { return obstacles; }
    const QPointF& getStart() const { return start; }
    const QPointF& getFinish() const { return finish; }

private:
    static const QSizeF SIZE;
    const QString name;
    QPointF start;
    QPointF finish;
    std::vector<Polygon> obstacles;
};

#endif // CONFIGURATIONSPACE_H
