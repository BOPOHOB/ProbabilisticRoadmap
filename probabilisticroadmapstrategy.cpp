#include "probabilisticroadmapstrategy.h"

#include <QDebug>
#include <QFile>
#include <QLineF>
#include <QTextStream>

#include <algorithm>
#include <set>
#include <cassert>
#include <random>
#include <functional>
#include <cmath>

#include "graph.h"
#include "motion.h"
#include "configurationspace.h"

ProbabilisticRoadmapStrategy::ProbabilisticRoadmapStrategy()
{

}

Graph ProbabilisticRoadmapStrategy::buildGraph(const ConfigurationSpace& space) const {
    std::random_device rd;
    std::mt19937_64 gen(rd());

    std::uniform_real_distribution<> yDistributuin(0, space.spaceSize().height());
    std::uniform_real_distribution<> xDistributuin(0, space.spaceSize().width());

    std::vector<QPointF> vertices{ space.getStart(), space.getFinish() };
    for (int i(0); i != initialSampleSize; ++i) {
        vertices.emplace_back(xDistributuin(gen), yDistributuin(gen));
        while (!space.isValid(vertices.back())) {
            vertices.back() = QPointF(xDistributuin(gen), yDistributuin(gen));
        }
    }

    for (const Polygon& p : space.getObstacles()) {
        std::vector<QPointF> sample(p.nearPointsSample(obstaclesSampleSize));
        sample.erase(std::remove_if(sample.begin(), sample.end(), [&space](const QPointF& p) {
            return !space.isValid(p);
        }), sample.end());
        vertices.insert(vertices.end(), sample.begin(), sample.end());
    }

    std::vector<std::set<int>> edges;
    edges.resize(vertices.size());
#ifndef M_PI
    static const double M_PI(acos(-1.));
#endif
    const double r(sqrt((static_cast<double>(entireGraphRadius) / initialSampleSize
                   * space.spaceSize().width() * space.spaceSize().height()) / M_PI));
    for (int i(0); i != vertices.size(); ++i) {
        const QPointF& cur(vertices.at(i));
        std::set<int>& allied(edges[i]);
        for (int j(0); j != vertices.size(); ++j) if (i != j) {
            if (QLineF(cur, vertices.at(j)).length() < r && space.isApparent(cur, vertices.at(j))) {
                allied.insert(j);
            }
        }
    }
    return std::move(Graph(std::move(vertices), std::move(edges)));
}

Motion ProbabilisticRoadmapStrategy::plane(const ConfigurationSpace& space) const
{
    const Graph graph(buildGraph(space));

#ifndef QT_NO_DEBUG
    qDebug() << "graph ready";
    QFile f(space.getName() + "_graph.json");
    f.remove();
    f.open(QFile::WriteOnly);
    QTextStream(&f) << graph;
#endif

    return graph.shortestPath(space.getStart(), space.getFinish());
}
