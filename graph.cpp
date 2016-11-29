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

Graph::Graph(std::vector<QPointF>&& vertices, std::vector<std::set<int> >&& edges)
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

    struct SetSet {
        std::map<Trace::mapped_type, Trace::key_type> a;
        std::set<Trace::key_type> b;

        SetSet(const int& startId)
            : a{ { std::make_pair(0., startId), startId } }
            , b{ startId } { }

        std::pair<Trace::mapped_type, Trace::key_type> pop() {
            std::pair<Trace::mapped_type, Trace::key_type> v(*a.begin());
            a.erase(a.begin());
            b.erase(v.second);
            return std::move(v);
        }

        void insert(int id, double d, int from) {
            b.insert(id);
            a.insert(std::make_pair(std::make_pair(d, from), id));
        }

        void hint(int id, double d, int from) {
            if (b.find(id) == b.end()) {
                insert(id, d, from);
            } else {
                std::map<Trace::mapped_type, Trace::key_type>::iterator it(a.begin());
                while (it->second != id) {
                    ++it;
                }
                if (it->first.first > d) {
                    a.erase(it);
                    b.erase(b.find(id));
                    insert(id, d, from);
                }
            }
        }

        bool empty() const noexcept { return a.empty(); }
    } inuse(startId);

    Trace used;

    while (!inuse.empty()) {
        const auto cur(inuse.pop());
        used.emplace(cur.second, cur.first);
        const QPointF& curPoint(vertices.at(cur.second));
        if (cur.second == endId) {
            return collectMotion(used, endId, startId);
        }

        for (const int& id : edges[cur.second]) if (used.find(id) == used.end()) {
            inuse.hint(id, cur.first.first + QLineF(curPoint, vertices.at(id)).length(), cur.second);
        }
    }

    return Motion();
}

Motion Graph::collectMotion(const Trace &map, const int end, const int start) const
{
    std::stack<int> stack;
    stack.push(end);
    while (stack.top() != start) {
        const Trace::const_iterator it(map.find(stack.top()));
        assert(it != map.end());
        stack.push(it->second.second);
    }

    Motion result;
    result.reserve(stack.size());
    while (!stack.empty()) {
        result.emplace_back(vertices[stack.top()]);
        stack.pop();
    }

#ifndef QT_NO_DEBUG
    qDebug() << "solved";
#endif
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
