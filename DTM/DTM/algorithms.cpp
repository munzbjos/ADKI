#include "algorithms.h"
#include "sortbyx.h"

int Algorithms::getPointLinePosition(QPoint3D &q,QPoint3D &p1,QPoint3D &p2)
{
    //Analyze point and line position
    //1 point in the left half plane
    //0 point in the right half plane
    //-1 point on the line
    double ux = p2.x() - p1.x();
    double uy = p2.y() - p1.y();

    double vx = q.x() - p1.x();
    double vy = q.y() - p1.y();

    //Test criteron
    double t = ux * vy - uy * vx;

    //Point in the left half plane
    if (t>0)
        return 1;

    //Point in the right halfplane
    if (t<0)
        return 0;

    //Colinear point
    return -1;
}

void Algorithms::circleCenterAndRadius(QPoint3D &p1, QPoint3D &p2, QPoint3D &p3, double &r, QPoint3D &s)
{
    //Center and radius of circle given by 3 points p1, p2, p3
    double k1 = p1.x() * p1.x() + p1.y() * p1.y();
    double k2 = p2.x() * p2.x() + p2.y() * p2.y();
    double k3 = p3.x() * p3.x() + p3.y() * p3.y();
    double k4 = p1.y() - p2.y();
    double k5 = p1.y() - p3.y();
    double k6 = p2.y() - p3.y();
    double k7 = p1.x() - p2.x();
    double k8 = p1.x() - p3.x();
    double k9 = p2.x() - p3.x();
    double k10 = p1.x() * p1.x();
    double k11 = p2.x() * p2.x();
    double k12 = p3.x() * p3.x();

    //Compute m: x-coordinate of circle center
    double m_numerator = -k12 * k4 + k11 * k5 - (k10 + k4 * k5) * k6;
    double m_denominator = -p3.x() * k4 + p2.x() * k5 - p1.x() * k6;
    double m = 0.5 * m_numerator / m_denominator;

    //Compute n: y-coordinate of circle center
    double n_numerator = -k1 * k9  + k2 * k8 - k3 * k7;
    double n_denominator = -p1.y() * k9  + p2.y()  * k8 - p3.y()  * k7;
    double n = 0.5 * n_numerator / n_denominator;

    //Compute circle radius
    r = sqrt((p1.x() - m) * (p1.x() - m) + (p1.y() - n) * (p1.y() - n));

    //Set m,n to s
    s.setX(m);
    s.setY(n);
}


int Algorithms::findDelaunayPoint(QPoint3D &pi, QPoint3D &pj, std::vector<QPoint3D> &points)
{
    //Find optimal Delaunay point (third vertex of of triangle)
    int i_min = -1;
    double r_min = INFINITY;

    //Browse all input points
    for (int i = 0; i < points.size(); i++)
    {
        //Check, whether points[i] different from pi, pj
        if((points[i]!=pi) && (points[i]!=pj))
        {
            //Point in the left halfplane
            if(getPointLinePosition(points[i], pi, pj)==1)
            {
                //Compute circle center and radius
                QPoint3D s;
                double r;
                circleCenterAndRadius(pi, pj, points[i], r, s);

                //Circle center in the right halplane
                if(getPointLinePosition(s, pi, pj)==0)
                    r = -r;

                //Actualize minimum
                if(r<r_min)
                {
                    r_min = r;
                    i_min = i;
                }

            }
        }
    }

    return i_min;
}


double Algorithms::dist(QPoint3D &p1, QPoint3D &p2)
{
     //Get Euclidean distance between two points
     double dx = (p1.x() - p2.x());
     double dy = (p1.y() - p2.y());

     return sqrt(dx*dx + dy*dy);
}


int Algorithms::getNearestpoint(QPoint3D &p, std::vector<QPoint3D> &points)
{
    //Find nearest point to p
    int i_min = 1;
    double d_min = dist(p, points[1]);

    //Browses all points
    for (unsigned int i = 2; i < points.size(); i++)
    {
        //Compute distance
        double d = dist(p, points[i]);

        //Actualize minimum i and distance
        if (d < d_min)
        {
            d_min=d;
            i_min=i;
        }
    }

    return i_min;
}


std::vector<Edge> Algorithms:: DT(std::vector<QPoint3D> &points)
{
    //Delaunay triangulation
    std::vector<Edge> dt;
    std::list<Edge> ael;

    //Sort by X
    sort(points.begin(), points.end(), sortByX());

    //Pivot
    QPoint3D q = points[0];

    //Nearest point
    int index = getNearestpoint(q,points);
    QPoint3D p_nearest = points[index];

    //Find optimal Delaunay point
    int i_o = findDelaunayPoint(q, p_nearest,points);

    //Create new edge
    Edge e(q, p_nearest);

    //No suitable point found in the left halfplane
    if (i_o == -1)
    {
        //Change orientation of the edge
        e.changeOrientation();

        //Find optimal Delauanay points once more
        i_o = findDelaunayPoint(e.getStart(), e.getEnd(), points);
    }

    //3rd vertex of triangle
    QPoint3D p_3 = points[i_o];

    //Create inicial triangle
    Edge e2(e.getEnd(), p_3);
    Edge e3(p_3, e.getStart());

    //Add triangle to DT
    dt.push_back(e);
    dt.push_back(e2);
    dt.push_back(e3);

    //Add edges to AEL
    ael.push_back(e);
    ael.push_back(e2);
    ael.push_back(e3);

    //Until ael is empty process candidate edges
    while (!ael.empty())
    {
        //Get last edge
        Edge edge1 = ael.back();
        ael.pop_back();

        //Change orientation
        edge1.changeOrientation();

        //Find optimal Delauanay point
        i_o = findDelaunayPoint(edge1.getStart(), edge1.getEnd(), points);

        //Optimal point found
        if (i_o != -1)
        {
            //3rd vertex of triangle
            QPoint3D p3 = points[i_o];

            //Create inicial triangle
            Edge edge2(edge1.getEnd(), p3);
            Edge edge3(p3, edge1.getStart());

            //Add triangle to DT
            dt.push_back(edge1);
            dt.push_back(edge2);
            dt.push_back(edge3);

            //Change orientation of edges
            edge2.changeOrientation();
            edge3.changeOrientation();

            //Update AEL
            updateAEL(edge2,ael);
            updateAEL(edge3,ael);
        }
    }

    return dt;
}


void Algorithms::updateAEL(Edge &e, std::list<Edge> &ael)
{
    //Update AEL
    std::list<Edge>::iterator ie = find(ael.begin(),ael.end(), e);

    //Edge is not in AEL
    if(ie == ael.end())
    {
        //Change orientation
        e.changeOrientation();

        //Add edge to AEL
        ael.push_back(e);
    }

    //Edge is already in list, erase
    else ael.erase(ie);
}

QPoint3D Algorithms::getContourPoint(QPoint3D &p1, QPoint3D &p2, double z)
{
    //Compute contour point
    double xa = (p2.x() - p1.x())/(p2.getZ() - p1.getZ()) * (z - p1.getZ()) + p1.x();
    double ya = (p2.y() - p1.y())/(p2.getZ() - p1.getZ()) * (z - p1.getZ()) + p1.y();
    QPoint3D A(xa, ya, z);

    return A;
}

std::vector<Edge> Algorithms::contourLines(std::vector<Edge> &dt, double z_min, double z_max, double dz)
{
    //Generate contour lines
    std::vector<Edge> contours;

    //Browse all triangles
    for (unsigned int i=0; i<dt.size(); i+=3)
    {
        //Get triangle vertices
        QPoint3D p1 = dt[i].getStart();
        QPoint3D p2 = dt[i].getEnd();
        QPoint3D p3 = dt[i+1].getEnd();

        // Find all plane - triangle intersections.
        for (double z = z_min; z <= z_max; z += dz)
        {
            // Get height differences
            double dz1 = z - p1.getZ();
            double dz2 = z - p2.getZ();
            double dz3 = z - p3.getZ();

            //Test criteria
            double t12 = dz1 * dz2;
            double t23 = dz2 * dz3;
            double t31 = dz3 * dz1;

            //Triangle in the plane(coplanar)
            if((dz1 == 0) && (dz2 == 0) && (dz3 == 0))
                continue;

            //Edge e12 of the triangle in the plane
            else if ((dz1 == 0) && (dz2 == 0))
                contours.push_back(dt[i]);

            //Edge e23 of the triangle in the plane
            else if ((dz2 == 0) && (dz3 == 0))
                contours.push_back(dt[i+1]);

            //Edge e31 of the triangle in the plane
            else if ((dz3 == 0) && (dz1 == 0))
                contours.push_back(dt[i+2]);

            //Egdes e12, e13 are intersected
            else if ((t12 < 0 && t23 <= 0) || (t12 <= 0 && t23 < 0))
            {
                //Compute intersection points
                QPoint3D A = getContourPoint(p1, p2, z);
                QPoint3D B = getContourPoint(p2, p3, z);

                // Create fragment of contour line.
                Edge e(A, B);
                contours.push_back(e);
            }

            //Egdes e23, e31 are intersected
            else if ((t23 < 0 && t31 <= 0) || (t23 <= 0 && t31 < 0))
            {
                //Compute intersection points
                QPoint3D A = getContourPoint(p2, p3, z);
                QPoint3D B = getContourPoint(p3, p1, z);

                // Create fragment of contour line.
                Edge e(A, B);
                contours.push_back(e);
            }

            //Egdes e31, e12 are intersected
            else if ((t31 < 0 && t12 <= 0) || (t31 <= 0 && t12 < 0))
            {
                //Compute intersection points
                QPoint3D A = getContourPoint(p3, p1, z);
                QPoint3D B = getContourPoint(p1, p2, z);

                // Create fragment of contour line
                Edge e(A, B);
                contours.push_back(e);
            }
        }
    }

    return contours;
}

double Algorithms::calculateSlope(QPoint3D &p1, QPoint3D &p2, QPoint3D &p3){

        //Calculate slope of the triangle
        double ux = p2.x() - p1.x();
        double uy = p2.y() - p1.y();
        double uz = p2.getZ() - p1.getZ();

        double vx = p3.x() - p1.x();
        double vy = p3.y() - p1.y();
        double vz = p3.getZ() - p1.getZ();

        //Calculate normal vector and its norm
        double nx = uy * vz - uz * vy;
        double ny = -(ux * vz - vx * uz);
        double nz = ux * vy - uy * vx;
        double nt = sqrt(nx * nx + ny * ny + nz * nz);

        return acos(nz / nt)*180/M_PI;

}
double Algorithms::calculateAspect(QPoint3D &p1, QPoint3D &p2, QPoint3D &p3){

        double ux = p2.x() - p1.x();
        double uy = p2.y() - p1.y();
        double uz = p2.getZ() - p1.getZ();

        double vx = p3.x() - p1.x();
        double vy = p3.y() - p1.y();
        double vz = p3.getZ() - p1.getZ();

        //Calculate normal vector and its norm
        double nx = uy * vz - uz * vy;
        double ny = -(ux * vz - vx * uz);

        return atan2(nx, ny) / M_PI * 180;
}

std::vector<Triangle> Algorithms:: analyzeDTM(std::vector<Edge> &dt)
{
    //Analyze DTM compute slope and aspect
    std::vector<Triangle> triangles;

    //Browse triangulation one by one
    for (unsigned int i = 0; i < dt.size(); i += 3)
    {
        //Vertices of the triangle
        QPoint3D p1 = dt[i].getStart();
        QPoint3D p2 = dt[i].getEnd();
        QPoint3D p3 = dt[i+1].getEnd();

        //Slope and aspect
        double fi = calculateSlope(p1, p2, p3);
        double alfa = calculateAspect(p1, p2, p3);

        //Create triangle
        Triangle t(p1, p2, p3, fi, alfa);

        //Add triangle to vector
        triangles.push_back(t);
    }

    return triangles;
}

