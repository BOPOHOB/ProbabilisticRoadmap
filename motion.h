#ifndef MOTION_H
#define MOTION_H

#include <vector>
#include <QPointF>
class QTextStream;
class QJsonArray;

struct Motion : std::vector<QPointF>
{
    QJsonArray toJsonArray() const;
};

QTextStream& operator<< (QTextStream& out, const Motion&);

#endif // MOTION_H
