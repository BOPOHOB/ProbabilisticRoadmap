#ifndef MOTION_H
#define MOTION_H

#include <vector>
#include <QPointF>
class QTextStream;

class Motion : public std::vector<QPointF>
{};

QTextStream& operator<< (QTextStream& out, const Motion&);

#endif // MOTION_H
