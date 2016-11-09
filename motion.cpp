#include "motion.h"
#include <QTextStream>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonDocument>
#include <QLineF>
#include <QDebug>

QTextStream& operator<< (QTextStream& out, const Motion& m)
{
    if (m.empty()) {
        return out;
    }
    QJsonArray result;

    QLineF line(m.front(), m.front());

    for (const QPointF& point : m) {
        line.setP2(point);
        result << QJsonObject({
                                  QPair<QString, QJsonValue>("x1", static_cast<double>(line.x1())),
                                  QPair<QString, QJsonValue>("y1", static_cast<double>(line.y1())),
                                  QPair<QString, QJsonValue>("x2", static_cast<double>(line.x2())),
                                  QPair<QString, QJsonValue>("y2", static_cast<double>(line.y2())),
                              });
        line.setP1(line.p2());
        result << QJsonObject({ QPair<QString, QJsonValue>("cx", static_cast<double>(point.x())),
                                QPair<QString, QJsonValue>("cy", static_cast<double>(point.y())) });
    }

    return out << QJsonDocument(result).toJson();
}
