#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <vector>
#include <list>
#include "qpoint3d.h"
#include "edge.h"
#include "triangle.h"



class Algorithms
{
public:
    Algorithms(){};
    int getPointLinePosition(QPoint3D &q, QPoint3D &p1, QPoint3D &p2);
    void circleCenterAndRadius(QPoint3D &p1, QPoint3D &p2, QPoint3D &p3, double &r, QPoint3D &s);
    int findDelaunayPoint(QPoint3D &pi, QPoint3D &pj, std::vector<QPoint3D> &points);
    double dist(QPoint3D &p1, QPoint3D &p2);
    int getNearestpoint(QPoint3D &p, std::vector<QPoint3D> &points);
    std::vector<Edge> DT(std::vector<QPoint3D> &points);
    void updateAEL(Edge &e, std::list<Edge> &ael);
    QPoint3D getContourPoint(QPoint3D &p1, QPoint3D &p2, double z);
    std::vector<Edge> contourLines(std::vector<Edge> &dt, double z_min, double z_max, double dz);
    double calculateAspect(QPoint3D &p1, QPoint3D &p2, QPoint3D &p3);
    double calculateSlope(QPoint3D &p1, QPoint3D &p2, QPoint3D &p3);
    std::vector<Triangle> analyzeDTM(std::vector<Edge> &dt);
};

#endif // ALGORITHMS_H
