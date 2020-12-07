#ifndef REMOVEBYANGLE_H
#define REMOVEBYANGLE_H

#include<QtGui>
#include<cmath>

class removeByAngle
{

private:
    QPoint q;

public:
    removeByAngle(QPoint &q_){q = q_;}
    bool operator ()(const QPoint &p1, const QPoint &p2)const
    {
        double sigma1 = atan2(p1.y()-q.y(),p1.x()-q.x());
        double sigma2 = atan2(p2.y()-q.y(),p2.x()-q.x());

        return sigma1 == sigma2;
    }
};

#endif // REMOVEBYANGLE_H
