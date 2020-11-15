#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <vector>
#include <QtGui>
#include "sortbyy.h"
#include "sortbyx.h"

class Algorithms
{

public:
    Algorithms();
        double getAngle(QPoint &p1,QPoint &p2,QPoint &p3,QPoint &p4);
        QPolygon jarvis(std::vector<QPoint> &points);
        QPolygon graham(std::vector<QPoint> &points);
        QPolygon qHull(std::vector<QPoint> &points);
        QPolygon sweepLine(std::vector<QPoint> &points);
        std::vector<QPoint> removeDuplicite(std::vector<QPoint> &points);
        int getPointLinePosition(QPoint &q,QPoint &p1,QPoint &p2);
        double getPointLineDist(QPoint &a, QPoint &p1, QPoint &p2);
        void qh(int s, int e, std::vector<QPoint> &points, QPolygon &ch);
        QPolygon strictlyConvexHull(QPolygon &ch);



};

#endif // ALGORITHMS_H
