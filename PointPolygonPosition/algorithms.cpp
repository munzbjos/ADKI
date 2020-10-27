#include "algorithms.h"
#include <vector>
#include <QtGui>
#include <iostream>
#include <cmath>

Algorithms::Algorithms()
{

}
/*
bool Algorithms::getDistance(QPoint &q, QPoint &p1, QPoint &p2)
{
    double p1p2 = sqrt((p1.x()-p2.x())*(p1.x()-p2.x())+(p1.y()-p2.y())*(p1.y()-p2.y()));
    double p1q = sqrt((p1.x()-q.x())*(p1.x()-q.x())+(p1.y()-q.y())*(p1.y()-q.y()));
    double p2q = sqrt((p2.x()-q.x())*(p2.x()-q.x())+(p2.y()-q.y())*(p2.y()-q.y()));
    double tol = 1.0e-1;

    //if distance between polygon vertex and chosen point are equal, give us 1
    if (p1p2-(p1q+p2q)>tol)
        return 0;
    else
        return 1;
} */
int Algorithms::getPointLinePosition(QPoint &q, QPoint &p1, QPoint &p2)
{
    //Get point and line position
    double ux = p2.x() - p1.x();
    double uy = p2.y() - p1.y();
    double vx = q.x() - p1.x();
    double vy = q.y() - p1.y();

    //Determinant ux, uy, vx, vy
    double t = ux * vy - uy * vx;

    //get distance
    double p1p2 = sqrt((p1.x()-p2.x())*(p1.x()-p2.x())+(p1.y()-p2.y())*(p1.y()-p2.y()));
    double p1q = sqrt((p1.x()-q.x())*(p1.x()-q.x())+(p1.y()-q.y())*(p1.y()-q.y()));
    double p2q = sqrt((p2.x()-q.x())*(p2.x()-q.x())+(p2.y()-q.y())*(p2.y()-q.y()));
    double tol = 4.0;

    //does the point lie within tolerance of vertex?
    if (((fabs(q.x()-p1.x())<tol) && (fabs(q.y()-p1.y())<tol)) || ((fabs(q.x()-p2.x())<tol) && (fabs(q.y()-p2.y())<tol)))
    {
        return 3;
    }

    //if distance between polygon vertex and chosen point are equal, give us 1
    if (fabs(p1p2-(p1q+p2q))<=tol)
    {
        return 2;
    }
    if (t>0)
        return 1;
    else
        return 0;

}
double Algorithms::getAngle(QPoint &p1, QPoint &p2, QPoint &p3, QPoint &p4)
{
    // Calculate Vector betwen 2 vectors.
    double ux = p2.x() - p1.x();
    double uy = p2.y() - p1.y();

    double vx = p4.x() - p3.x();
    double vy = p4.y() - p3.y();

    //Norms
    double nu = sqrt(ux * ux + uy * uy);
    double nv = sqrt(vx * vx + vy * vy);

    //Dot product
    double scalar = ux * vx + uy * vy;

    return fabs(acos(scalar/(nu*nv)));

}
int Algorithms::getPositionWinding(QPoint &q, std::vector<QPoint> &pol)
{
    // Analyze Position of the Point and the Polygon
    double Omega = 0.0;

    // Tolerance
    const double eps = 1.0e-6;

    // The size of polygon
    const int n = pol.size();

    //Browse all points of polygon
    for (int i = 0; i < n; i++){

        //Measure angle
        double om = getAngle(pol[i],q,pol[(i+1)%n],q);
        //double om = getAngle(q,pol[i],q,pol[(i+1)%n]);

        //Get orientation of the point and the polygon edge
        int orientation = getPointLinePosition(q, pol[i], pol[(i+1)%n]);

        //Point in the left half plane
        if (orientation == 1)
            Omega += om;

        //if on border return 2
        else if (orientation == 2)
            return 2;
        else if (orientation == 3)
            return 3;
        //Point in the right half plane
        else
            Omega -= om;
        // else
        //   Omega = -1
    }

    //Point q inside polygon
    if (fabs(fabs(Omega) - 2 * M_PI) <= eps)
        return 1;
    //Point outside polygon
    return 0;
}
int Algorithms::getPositionRay(QPoint &q, std::vector<QPoint> &pol)
{
    // Analyze Position of the Point and the Polygon
    int k = 0;

    // Size of polygon
    const int n = pol.size();

    for (int i = 1; i < n+1 ; i++)
    {
        //reduce coordinates
        double xir = pol[i%n].x() - q.x();
        double yir = pol[i%n].y() - q.y();
        double xiir = pol[i-1].x() - q.x();
        double yiir = pol[i-1].y() - q.y();

        //if on border, return 2
        int orientation = getPointLinePosition(q, pol[i%n], pol[(i+1)%n]);
        if (orientation == 2)
            return 2;
        else if (orientation == 3)
            return 3;


        //Does segment (pi-1, pi) intersect ray
        if (((yir > 0) && (yiir <= 0)) || ((yiir > 0) && (yir <= 0)))
        {

            // Compute x coordinate ofintersection
            double xm = (xir * yiir - xiir * yir)/(yir - yiir);

            // Point q in the right halfplane
            if (xm > 0)
                k +=1;
        }
        /*
        //Assign coordinates
        yir = yiir;
        xir = xiir; */
    }

    //Odd/even amount of intersections
    return k%2;
}
