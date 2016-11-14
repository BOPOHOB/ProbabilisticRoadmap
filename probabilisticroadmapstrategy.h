#ifndef PRMSTRATEGY_H
#define PRMSTRATEGY_H

#include <QPointF>
#include <vector>

class Graph;
struct Motion;
class ConfigurationSpace;

class ProbabilisticRoadmapStrategy
{
    int initialSampleSize = 100;
    int obstaclesSampleSize = 25;
    int entireGraphRadius = 15;

    Graph buildGraph(const ConfigurationSpace& space) const;
public:
    ProbabilisticRoadmapStrategy();

    Motion plane(const ConfigurationSpace&) const;
};

#endif // PRMSTRATEGY_H
