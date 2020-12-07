#ifndef SORTBYANGLE_H
#define SORTBYANGLE_H
#include<QtGui>
#include<cmath>

class sortByAngle
{
private:
    QPoint q;

public:
    sortByAngle(QPoint q_){q = q_;}

    bool operator ()(QPoint &p1, QPoint &p2)
    {
        double sigma1 = atan2(p1.y()-q.y(),p1.x()-q.x());
        double sigma2 = atan2(p2.y()-q.y(),p2.x()-q.x());
        double d1 = (p1.y()-q.y())*(p1.y()-q.y())+(p1.x()-q.x())*(p1.x()-q.x());
        double d2 = (p2.y()-q.y())*(p2.y()-q.y())+(p2.x()-q.x())*(p2.x()-q.x());

        return sigma1 < sigma2 || (sigma1 == sigma2) && (d1 > d2);
    }
};

#endif // SORTBYANGLE_H
