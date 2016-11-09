#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "sourcedata.h"
#include "probabilisticroadmapstrategy.h"
#include "configurationspace.h"
#include "motion.h"

using namespace std;

int main()
{
    const SourceData data("../");
    for (const ConfigurationSpace& i : data) {
        QFile f(i.getName() + "_solve.json");
        f.remove();
        f.open(QFile::WriteOnly);
        QTextStream(&f) << ProbabilisticRoadmapStrategy().plane(i);
        f.close();
        qDebug() << i.getName() << "solution ready";
    }
    return 0;
}
