#include "algorithms.h"
#include "sortbyy.h"
#include "sortbyx.h"
#include "sortbyangle.h"
#include "removebyangle.h"
#include "removebycoords.h"
#include <vector>
#include <deque>
#include <stack>
#include <QtGui>
#include <iostream>
#include <cmath>

Algorithms::Algorithms()
{

}

double Algorithms::getPointLineDist(QPoint &a, QPoint &p1, QPoint &p2){
    //Calculate point and line distance
    double numerator = a.x() * (p1.y() - p2.y()) + p1.x() * (p2.y() - a.y()) + p2.x() * (a.y() - p1.y());
    double dx = p2.x() - p1.x();
    double dy = p2.y() - p1.y();
    return fabs(numerator)/sqrt(dx * dx + dy * dy);
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
int Algorithms::getPointLinePosition(QPoint &q,QPoint &p1,QPoint &p2)
{
//Analyze point and line position
//1 point in the left half plane
//0 point in the right half plane
//2 point on the line
double ux = p2.x() - p1.x();
double uy = p2.y() - p1.y();

double vx = q.x() - p1.x();
double vy = q.y() - p1.y();

double t = ux * vy - uy * vx;

//Point in the left half plane
if (t>0)
    return 1;
if (t<0)
    return 0;
return 2;
}

void Algorithms::qh(int s, int e, std::vector<QPoint> &points, QPolygon &ch){

    //Recursive procedure
    int i_max = -1;
    double d_max = 0;

    // Browse all points
    for (int i = 2; i < points.size(); i++)
    {
       // Is the point in the right half plane
        if (getPointLinePosition(points[i], points[s], points[e]) == 0)
        {
            double distance = getPointLineDist(points[i], points[s], points[e]);

            //Actualization of d_max
            if(distance > d_max){
                d_max = distance;
                i_max = i;
            }
        }
    }

    //Suitable point has been found
    if(i_max != -1)
    {
        qh(s, i_max, points, ch);
        ch.push_back(points[i_max]);
        qh(i_max, e, points, ch);
    }

}

QPolygon Algorithms::jarvis(std::vector<QPoint> &points)
{
    //Convex hull
    QPolygon ch;

    //Sort points by Y
    std::sort(points.begin(), points.end(), sortByY());
    QPoint q=points[0];
    QPoint r(0,q.y());

    //Initialize points pj, pjj
    QPoint pj = q;
    QPoint pjj = r;

    //Add q into Convex Hull
    ch.push_back(q);

    //Find all points of Convex hull
    do
    {
        //Initialize i_max, om_max
        int i_max = -1;
        double o_max = 0;


        //Find suitable point maximazing angle omega
        for(int i = 0; i < points.size(); i++)
        {
            //compute omega
            double omega = getAngle(pj, pjj, pj, points[i]);

            // Actualize maximum.
            if (omega > o_max)
            {
                o_max = omega;
                i_max = i;
            }
        }

        //Add point to convex hull
        ch.push_back(points[i_max]);

        //Assign points
        pjj = pj;
        pj = points[i_max];

    } while (!(pj == q));

    ch = strictlyConvexHull(ch);
    return ch;
}
QPolygon Algorithms::graham(std::vector<QPoint> &points)
{
    //
    std::deque<QPoint> ch;

    //find pivot q
    QPoint  q = *min_element(points.begin(), points.end(), sortByY());
    //Sort points by their dirrections
    std::sort(points.begin(), points.end(), sortByAngle(q));
    //remove duplicate points
    auto it = std::unique(points.begin(), points.end(), removeByAngle(q));
    //trip vector
    points.resize(it - points.begin());
    //Add 2 points to ch
    ch.push_front(q);
    ch.push_front(points[0]);
    //Process all points
    int j = 2, n = points.size();

    while (j<n)
    {
        //remove point
        QPoint p1 = ch.front();
        ch.pop_front();
        //Get point on the top
        QPoint p2 = ch.front();
        //Is points[j] in the left half plane?
        if (getPointLinePosition(points[j],p2,p1) == 1)
        {
            //push points back to stack
            ch.push_front(p1);
            //push next point to stack
            ch.push_front(points[j]);
            j++;
        }

    }
    //QPolygon ch2;
    //ch2 -> ch;
    QVector<QPoint> ch2 (ch.begin(),ch.end());
    QPolygon ch3(ch2);

    ch3 = strictlyConvexHull(ch3);
    return ch3;


}

QPolygon Algorithms::qHull(std::vector<QPoint> &points)
{
    //Convex Hull using quick hull method (Global procedure)
    QPolygon ch;
    std::vector<QPoint> upoints, lpoints;

    // Sort by X.
    std::sort(points.begin(), points.end(), sortByX());
    QPoint q1 = points[0];
    QPoint q3 = points.back();

    //Add q1, q3 to upoints, lpoints
    upoints.push_back(q1);
    upoints.push_back(q3);

    lpoints.push_back(q1);
    lpoints.push_back(q3);

    // Split to upoints / lpoints
    for(unsigned int i=0; i < points.size();i++){

        //Add to the upper part
        if(getPointLinePosition(points[i], q1, q3) == 1)
            upoints.push_back(points[i]);

        //Add to the lower part
        else if (getPointLinePosition(points[i], q1, q3) == 0)
            lpoints.push_back(points[i]);
    }

    //Call recursive function
    ch.push_back(q3);
    qh(1, 0, upoints, ch);
    ch.push_back(q1);
    qh(0, 1, lpoints, ch);

    ch = strictlyConvexHull(ch);
    return ch;
}

QPolygon Algorithms::sweepLine(std::vector<QPoint> &points)
{
    //Convex Hull using sweepline Algorithm
    QPolygon ch;
    auto it = std::unique(points.begin(), points.end(), removeByCoords());
    //trip vector
    points.resize(it - points.begin());
    int m = points.size();

    //Create ist of predecessors and successors
    std::vector<int> p(m);
    std::vector<int> n(m);

    //Sort points by X
    std::sort(points.begin(),points.end(),sortByX());

    // Create initial aproximation
    n[0] = 1;
    n[1] = 0;
    p[0] = 1;
    p[1] = 0;

    // Process all points according to x coordinates
    for (int i = 2; i < m; i++)
    {
        //Point i lies in the upper half plane
        if(points[i].y() >= points[i-1].y())
        {
            //Link i and its predecessor and successor
            p[i] = i-1;
            n[i] = n[i-1];
        }

        //Point i lies in the lower half plane
        else
        {
            //Link i and its predecessor and successor
            p[i] = p[i-1];
            n[i] = i - 1;
        }

        //Link predecessor and successor and i
        p[n[i]] = i;
        n[p[i]] = i;

        //Fix upper tangent
        while (getPointLinePosition(points[n[n[i]]], points[i], points[n[i]]) == 0)
        {
            //Change predecessor and successor
            p[n[n[i]]] = i;
            n[i] = n[n[i]];
        }

        //Fix lower tangent
        while (getPointLinePosition(points[p[p[i]]], points[i], points[p[i]]) == 1)
        {
            //Change predecessor and successor
            n[p[p[i]]] = i;
            p[i] = p[p[i]];
        }
    }

    //Convert successors to ch
    ch.push_back(points[0]);

    //Second point of CH
    int index = n[0];

    //Repeat until the first point is found
    while(index != 0)
    {
        //Add to the polygon
        ch.push_back(points[index]);

        //Find its successor
        index = n[index];
    }
    ch = strictlyConvexHull(ch);

    return ch;
}


QPolygon Algorithms::strictlyConvexHull(QPolygon &ch)
{
    //Strictly Convex Hull
    QPolygon strict_ch;

    //Check if the first and the last points are the same in convex hull
    if (ch[0] == ch.back())
        ch.removeLast();

    int n = ch.size();

    //Process all points in convex hull
    for (int i = 0; i < n; i++)
    {
        //Check if three points are not collinear
        if (getPointLinePosition(ch[(i+2)%n], ch[i], ch[(i+1)%n]) != 2)
            strict_ch.push_back(ch[(i+1)%n]);
    }

    return strict_ch;
}
