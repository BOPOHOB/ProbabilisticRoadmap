#ifndef PRMSTRATEGY_H
#define PRMSTRATEGY_H

#include <QPointF>
#include <vector>

class Graph;
class Motion;
class ConfigurationSpace;

class ProbabilisticRoadmapStrategy
{
    int initialSampleSize = 100;
    int obstaclesSampleSize = 45;
    int entireGraphRadius = 15;

    Graph buildGraph(const ConfigurationSpace& space) const;
public:
    ProbabilisticRoadmapStrategy();

    Motion plane(const ConfigurationSpace&) const;
};

#endif // PRMSTRATEGY_H
