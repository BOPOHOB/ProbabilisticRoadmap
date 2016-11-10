#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>

#include "sourcedata.h"
#include "probabilisticroadmapstrategy.h"
#include "motion.h"

using namespace std;

int main()
{
    const SourceData data("../");
    for (const ConfigurationSpace& i : data) {
        QFile f(i.getName() + "_solve.json");
        f.remove();
        f.open(QFile::WriteOnly);
        QJsonArray arr;
        for (int j(15); j--; qDebug() << j) {
            for (const QJsonValue& k : ProbabilisticRoadmapStrategy().plane(i).toJsonArray()) {
                arr.append(k);
            }
        }
        QTextStream(&f) << QJsonDocument(arr).toJson();
        f.close();
        qDebug() << i.getName() << "solution ready";
    }
    return 0;
}
