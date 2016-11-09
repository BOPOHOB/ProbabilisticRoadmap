#ifndef GRAPH_H
#define GRAPH_H

#include <QPointF>
#include <vector>
#include <set>
#include <map>

class QTextStream;
struct Motion;

class Graph
{
    int indexOf(const QPointF&) const;
public:
    Graph(const std::vector<QPointF>&, const std::vector<std::set<int>>&);
    Graph(std::vector<QPointF>&&, std::vector<std::set<int>>&&);

    Motion shortestPath(const QPointF& begin, const QPointF& end) const;

private:
    std::vector<QPointF> vertices;
    std::vector<std::set<int>> edges;
    friend QTextStream& operator<<(QTextStream&, const Graph&);

    Motion collectMotion(const std::map<int, double> &, const int, const int start) const;
};

QTextStream& operator<<(QTextStream&, const Graph&);

#endif // GRAPH_H
