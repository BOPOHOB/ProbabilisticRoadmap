#ifndef POLYGON_H
#define POLYGON_H

#include <QPolygonF>
#include <functional>

class QJsonObject;
class ConfigurationSpace;

class Polygon : public QPolygonF
{
    struct Vector2D {
        const QPointF& a;
        const QPointF& c;
        Vector2D(const QPointF& x, const QPointF& y) : a(x), c(y) {}

        double k() const { return (c.y() - a.y()) / (c.x() - a.x()); }
        double b(const double& k) const { return a.y() - k * a.x(); }
        bool holdX(const double& x) const { return std::max(a.x(), c.x()) > x && std::min(a.x(), c.x()) < x; }
        bool holdY(const double& y) const { return std::max(a.y(), c.y()) > y && std::min(a.y(), c.y()) < y; }
    };
    static std::function<bool(const Vector2D&)> intersectionTester(const Vector2D&);

    QPointF center;

    static QPointF toPoint(const QJsonObject&);
public:
    Polygon(const QJsonObject&);
    Polygon() {}

    bool isInside(const QPointF&) const;

    bool isBetween(const QPointF&, const QPointF&) const;
    const QPointF& getCenter() const { return center; }
    std::vector<QPointF> nearPointsSample(size_t count) const;

    friend class ConfigurationSpace;
};

#endif // POLYGON_H
