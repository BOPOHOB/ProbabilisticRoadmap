#include "graph.h"

#include <QTextStream>
#include <QPair>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>
#include <QLineF>
#include <QDebug>
#include <utility>
#include <stack>
#include <cassert>

#include "motion.h"

Graph::Graph(const std::vector<QPointF>& vertices, const std::vector<std::set<int>>& edges)
    : vertices(vertices)
    , edges(edges)
{
}

Graph::Graph(std::vector<QPointF>&& vertices, std::vector<std::set<int>>&& edges)
    : vertices(std::move(vertices))
    , edges(std::move(edges))
{
}

Motion Graph::shortestPath(const QPointF& begin, const QPointF& end) const
{
    int startId(indexOf(begin));
    int endId(indexOf(end));
    if (startId == vertices.size() || endId == vertices.size()) {
        return Motion();
    }

    auto distance([](const QPointF& a, const QPointF& b)->double{
        return sqrt(pow(a.x() - b.x(), 2) + pow(a.y() - b.y(),2));
    });

    std::map<int, double> used;

    struct SetSet {
        std::map<double, int> a;
        std::set<int> b;

        SetSet(const int& startId)
            : a{ { 0., startId} }
            , b{ startId } { }

        std::pair<double, int> pop() {
            std::pair<double, int> v(*a.begin());
            a.erase(a.begin());
            b.erase(v.second);
            return std::move(v);
        }

        void suggest(int id, double d) {
            if (b.find(id) == b.end()) {
                b.insert(id);
                a[d] = id;
            } else {
                std::map<double, int>::iterator it(a.begin());
                while (it != a.end() && it->second != id) {
                    ++it;
                }
                if (it->first > d) {
                    a.erase(it);
                    a.insert(std::pair<double, int>(d, id));
                }
            }
        }

        bool empty() const noexcept { return a.empty(); }
    } inuse(startId);

    while (!inuse.empty()) {
        std::pair<double, int> cur(inuse.pop());
        used[cur.second] = cur.first;
        const QPointF& curPoint(vertices.at(cur.second));

        for (const int& id : edges[cur.second]) if (used.find(id) == used.end()) {
            if (id == endId) {
                return collectMotion(used, endId, startId);
            }
            inuse.suggest(id, cur.first + distance(curPoint, vertices.at(id)));
        }
    }

    return Motion();
}

Motion Graph::collectMotion(const std::map<int, double>& map, const int end, const int start) const
{
    std::stack<int> stack;
    stack.push(end);
    while (stack.top() != start) {
        const std::set<int> cur(edges[stack.top()]);
        std::pair<int, double> min(-1, std::numeric_limits<double>::infinity());
        for (const int& i : cur) {
            const std::map<int, double>::const_iterator val(map.find(i));
            if (val != map.end() && min.second > val->second) {
                min = *val;
            }
        }
        assert(min.first != -1);
        stack.push(min.first);
    }

    Motion result;
    result.reserve(stack.size());
    while (!stack.empty()) {
        if (vertices.size() <= stack.top()) {
            qFatal((QString("vertices fail") + vertices.size() + stack.top()).toStdString().c_str());
        }
        result.emplace_back(vertices[stack.top()]);
        stack.pop();
    }

    return std::move(result);
}

int Graph::indexOf(const QPointF&p) const
{
    const std::vector<QPointF>::const_iterator end(vertices.end());
    std::vector<QPointF>::const_iterator it(vertices.begin());

    while (it != end && *it != p) {
        ++it;
    }
    return it - vertices.begin();
}

QTextStream& operator<<(QTextStream& out, const Graph& g)
{
    QJsonArray result;
    for (int i(0); i != g.edges.size(); ++i) {
        for (int j : g.edges[i]) if (i > j) {
            const QLineF line(g.vertices[i], g.vertices[j]);
            result << QJsonObject({
                                      QPair<QString, QJsonValue>("x1", static_cast<double>(line.x1())),
                                      QPair<QString, QJsonValue>("y1", static_cast<double>(line.y1())),
                                      QPair<QString, QJsonValue>("x2", static_cast<double>(line.x2())),
                                      QPair<QString, QJsonValue>("y2", static_cast<double>(line.y2())),
                                  });
        }
    }
    for (const QPointF& p : g.vertices) {
        result << QJsonObject({ QPair<QString, QJsonValue>("cx", static_cast<double>(p.x())),
                                QPair<QString, QJsonValue>("cy", static_cast<double>(p.y())) });
    }
    return out << QJsonDocument(result).toJson();
}
