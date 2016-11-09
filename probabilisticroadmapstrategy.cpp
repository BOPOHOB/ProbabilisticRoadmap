#include "probabilisticroadmapstrategy.h"
#include <random>
#include <functional>
#include "configurationspace.h"
#include "motion.h"
#include <QDebug>
#include <algorithm>
#include <set>
#include <QFile>
#include <QTextStream>
#include <cassert>
#include "graph.h"

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
    auto distance([](const QPointF& a, const QPointF& b)->double{
        return sqrt(pow(a.x() - b.x(), 2) + pow(a.y() - b.y(),2));
    });
#ifndef M_PI
    static const double M_PI(acos(-1.));
#endif
    const double r(sqrt((static_cast<double>(entireGraphRadius) / initialSampleSize
                   * space.spaceSize().width() * space.spaceSize().height()) / M_PI));
    for (int i(0); i != vertices.size(); ++i) {
        const QPointF& cur(vertices.at(i));
        std::set<int>& allied(edges[i]);
        for (int j(0); j != vertices.size(); ++j) if (i != j) {
            if (distance(cur, vertices.at(j)) < r && space.isApparent(cur, vertices.at(j))) {
                allied.insert(j);
            }
        }
        if (allied.size() > entireGraphRadius) {
            while (allied.size() >= entireGraphRadius) {
                std::set<int>::iterator it(allied.begin());
                for (int i(std::uniform_int_distribution<>(0, allied.size() - 1)(gen)); i; --i) {
                    ++it;
                }
                allied.erase(it);
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
