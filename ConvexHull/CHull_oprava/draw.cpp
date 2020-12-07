#include "draw.h"
#include <QtGui>
#include <QDebug>
#include <QMessageBox>
#include "widget.h"
#include <cmath>
#include <QRandomGenerator>
#include "algorithms.h"
#include "removebycoords.h"

Draw::Draw(QWidget *parent) : QWidget(parent)
{

}

void Draw::mousePressEvent(QMouseEvent *e)
{
    //Get coordinates
    int x = e->pos().x();
    int y = e->pos().y();

    //Addppoint to the list
    QPoint p(x,y);
    points.push_back(p);

    repaint();
}

void Draw::paintEvent(QPaintEvent *e)
{
    //Start draw
    QPainter qp(this);
    qp.begin(this);

    //draw points
    int r = 1;
    for (unsigned int i = 0; i < points.size(); i++)
    {
        qp.drawEllipse(points[i].x()-r,points[i].y()-r, 2*r, 2*r);
    }

    //Draw points in the Convex Hull
    int r2 = 4;
    for (unsigned int i = 0; i < strict_points.size();i++)
    {
        qp.setPen(Qt::blue);
        qp.drawEllipse(strict_points[i].x()-r2,strict_points[i].y()-r2,2*r2,2*r2);
    }

    //Draw polygons
    qp.setPen(Qt::red);
    qp.drawPolygon(ch);

    qp.end();
}

std::vector <QPoint> Draw::generatePoints(int n, int height, int width)
{

    std::vector <QPoint> points;

    for (int i = 0; i < n; i++)
    {
        double x = ((float)rand() / (float)RAND_MAX )*width;
        double y = ((float)rand() / (float)RAND_MAX )*height;

    points.push_back(QPoint(x,y));
    }
    
    std::sort(points.begin(), points.end(), sortByX());

    //remove duplicate points
    auto it = std::unique(points.begin(), points.end(), removeByCoords());
    //trip vector
    points.resize(it - points.begin());

    return points;
}

std::vector <QPoint> Draw::generateGrid(int n, int height, int width)
{
    std::vector <QPoint> points;
    if(n<4){
        QMessageBox msgBox;
        msgBox.setText("Warning: The input has to be at least 4 to make a grid.");
        msgBox.exec();
        return points;
    }
    QPoint q;
    q.setX(4);
    q.setY(4);

    n = ceil(sqrt(n));

    for (int i = 0; i < n; i++)
    {
        for (int i = 0; i < n; i++)
        {
            q.setX((q.x()) + (height)/n);
            points.push_back(q);
        }
        q.setX(4);
        q.setY((q.y()) + (height)/n);
    }


    return points;
}


std::vector<QPoint> Draw::generateCircle(int n, int height, int width)
{
    std::vector<QPoint> points;
    QPoint p;
    QPoint center(width/2,height/2);
    //QPoint center(450,500);
    double radius = n/3000;
    double fi = (2*M_PI)/n;

    for(int i = 0;i<n;i++)
    {

        p.setX(center.x() + radius*cos(i*fi));
        p.setY(center.y() + radius*sin(i*fi));
        points.push_back(p);
    }
    return points;

}

std::vector<QPoint> Draw::generateSquare(int n, int height)
{
    std::vector<QPoint> points;

    if(n<4){
        QMessageBox msgBox;
        msgBox.setText("Warning: The input has to be at least 4 to make a square.");
        msgBox.exec();
        return points;
    }

    if(!(n%4==0)){
        QMessageBox msgBox;
        msgBox.setText("Warning: The input has to be dividing by 4 to make a square. That's why the input is reduced to appropriate number");
        msgBox.exec();
    }

    QPoint p(10,10);
    double length = height-15;

    QPoint p1(p.x()+length,p.y());
    QPoint p2(p.x()+length,p.y()+length);
    QPoint p3(p.x(),p.y()+length);
    points.push_back(p);
    points.push_back(p1);
    points.push_back(p2);
    points.push_back(p3);

    double length_small = length/(n/4);
    for(int i = 0;i < n/4;i++)
    {
        points.push_back(QPoint(p.x()+(i*length_small),p.y()));
        points.push_back(QPoint(p.x()+length,p.y()+(i*length_small)));
        points.push_back(QPoint(p.x()+(i*length_small),p.y()+length));
        points.push_back(QPoint(p.x(),p.y()+(i*length_small)));
    }

    return points;
}

std::vector<QPoint> Draw::generateEllipse(int n, int height, int width)
{
    std::vector<QPoint> points;
    QPoint p;
    QPoint center(width/2,height/2);
    int r = rand() %2 +1;
    double a;
    double b;
    if (r==1)
    {
        a = width/3;
        b = height/4;
    }

    else
    {
        a = width/4;
        b = height/3;
    }
    double fi = (2*M_PI)/n;

    for(int i = 0;i<n;i++)
    {

        p.setX(center.x() + a*cos(i*fi));
        p.setY(center.y() + b*sin(i*fi));
        points.push_back(p);
    }
    return points;
}

