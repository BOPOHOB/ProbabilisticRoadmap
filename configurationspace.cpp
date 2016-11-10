#include "configurationspace.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QRectF>

const QSizeF ConfigurationSpace::SIZE = QSizeF(50,30);

ConfigurationSpace::ConfigurationSpace(const QJsonObject& o, const QString & name)
    : name(name)
    , start(Polygon::toPoint(o.value("start").toObject()))
    , finish(Polygon::toPoint(o.value("finish").toObject()))
{
    for (const QJsonValue& v : o.value("polygons").toArray()) {
        obstacles.emplace_back(v.toObject());
    }
}

bool ConfigurationSpace::isValid(const QPointF& point) const
{
    if (point.x() < 0 || point.y() < 0 || SIZE.width() < point.x() || SIZE.height() < point.y()) {
        return false;
    }
    for (const Polygon& p : obstacles) {
        if (p.isInside(point)) {
            return false;
        }
    }
    return true;
}

bool ConfigurationSpace::isApparent(const QPointF& a, const QPointF& b) const
{
    for (const Polygon& p : obstacles) {
        if (p.isBetween(a, b)) {
            return false;
        }
    }
    return true;
}
