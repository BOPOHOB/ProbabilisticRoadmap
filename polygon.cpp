#include "polygon.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QLineF>
#include <QDebug>

#include <cassert>
#include <random>
#include <utility>
#include <ctgmath>
#include <cmath>

Polygon::Polygon(const QJsonObject &points)
    : center(toPoint(points))
{
    for (const QJsonValue& point : points.value("vertices").toArray()) {
        this->push_back(toPoint(point.toObject()));
    }
}

QPointF Polygon::toPoint(const QJsonObject&o)
{
    return QPointF(o.value("x").toDouble(), o.value("y").toDouble());
}

std::function<bool (const Polygon::Vector2D &)> Polygon::intersectionTester(const Vector2D& a)
{
    const double k1(a.k());
    const double b1(a.b(k1));

    return [a, k1, b1](const Vector2D& b)->bool {
        const double k2(b.k());
        const double b2(b.b(k2));
        if (!std::isfinite(k1) && !std::isfinite(k2)) {
            return qFuzzyIsNull(a.a.x() - b.a.x()) && std::max(a.a.y(), a.c.y()) > std::min(b.a.y(), b.c.y()) && std::min(a.a.y(), a.c.y()) < std::max(b.a.y(), b.c.y());
        } else if (!std::isfinite(k2)) {
            return b.holdY(k1 * b.a.x() + b1) && a.holdX(b.a.x());
        } else if (!std::isfinite(k1)) {
            return a.holdY(k2 * a.a.x() + b2) && b.holdX(a.a.x());
        } else {
            if (k1 != k2) {
                const double x((b2 - b1) / (k1 - k2));
                return a.holdX(x) && b.holdX(x);
            }
            return false;
        }
    };
}

bool Polygon::Vector2D::on(const QPointF& p) const
{
    if (std::isfinite(k())) {
        return qFuzzyIsNull(k() * p.x() + b(k()) - p.y()) && holdX(p.x());
    } else {
        return qFuzzyIsNull(p.x() - a.x()) && holdY(p.y());
    }
}

bool Polygon::isInside(const QPointF& inspected) const
{
    if (!this->boundingRect().contains(inspected)) {

        return false;
    }

    int count(0);
    if (first() == inspected) {
        return false;
    }

    const Vector2D underTest(center, inspected);
    auto test(intersectionTester(underTest));

    for (int i(1); i != this->size(); ++i) {
        if (inspected == at(i)) {
            return false;
        }
        count += test(Vector2D(at(i), at(i - 1)));
    }
    count += test(Vector2D(first(), last()));
    for (const QPointF& vertex : *this) {
        count += underTest.on(vertex);
    }

    return !(count % 2);
}

bool Polygon::isBetween(const QPointF& a, const QPointF& b) const
{
    auto test(intersectionTester(Vector2D(a, b)));

    for (int i(1); i != this->size(); ++i) {
        const Vector2D cur(at(i), at(i - 1));
        if (!cur.on(a) && !cur.on(b) && cur.a != a && cur.a != b && cur.c != a && cur.c != b) {
            if (test(cur)) {
                return true;
            }
        }
    }

    return false;
}

std::vector<QPointF> Polygon::nearPointsSample(size_t count) const
{
    double rMax(0.0);
    double rMin(std::numeric_limits<double>::max());
    for (const QPointF& i : *this) {
        const double dest(QLineF(center, i).length());
        rMax = std::max(dest, rMax);
        rMin = std::min(dest, rMin);
    }

    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_real_distribution<> rDistributuin(rMin, rMax * sqrt(2.));
    std::uniform_real_distribution<> phiDistributuin(0, 2 * acos(-1.));

    std::vector<QPointF> sample;
    sample.reserve(count);
    while (count--) {
        double r(rDistributuin(gen));
        double phi(phiDistributuin(gen));
        sample.emplace_back(r * sin(phi), r * cos(phi));
        sample.back() += center;
        while (isInside(sample.back())) {
            r = rDistributuin(gen);
            phi = phiDistributuin(gen);
            sample.back() = QPointF(r * sin(phi), r * cos(phi));
            sample.back() += center;
        }
    }
    return std::move(sample);
}
