#ifndef REMOVEBYCOORDS_H
#define REMOVEBYCOORDS_H
#include<QtGui>
#include<cmath>

class removeByCoords
{    
public:
    removeByCoords() {};

    bool operator ()(QPoint &p1, QPoint &p2)
    {

    double d1 = (p1.y()-p2.y())*(p1.y()-p2.y())+(p1.x()-p2.x())*(p1.x()-p2.x());


    return fabs(d1) < 0.1;
    }
};

#endif // REMOVEBYCOORDS_H
