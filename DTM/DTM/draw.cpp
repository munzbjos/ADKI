#include "draw.h"

Draw::Draw(QWidget *parent) : QWidget(parent)
{

}

void Draw::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.begin(this);

    //Draw points
    for (unsigned int i = 0; i < points.size(); i++)
    {
        painter.drawEllipse(points[i].x() - 1, points[i].y() - 1, 2, 2);
    }

    //Draw edges
    QPen p(Qt::green, 1);
    painter.setPen(p);

    for (unsigned int i = 0; i < dt.size(); i++)
    {
        painter.drawLine(dt[i].getStart(), dt[i].getEnd());
    }

    //Draw countour lines
    QPen q(Qt::gray, 1);
    painter.setPen(q);

    for (unsigned int i = 0; i < contours.size(); i++)
    {
        painter.drawLine(contours[i].getStart(), contours[i].getEnd());
    }

    //Draw slope
    double k = 255.0 / 180;
    for (Triangle t : dtm)
    {
        //Get triangle vertices
        QPoint3D p1 = t.getP1();
        QPoint3D p2 = t.getP2();
        QPoint3D p3 = t.getP3();
        int slope = 255 - t.getSlope() * k;

        //Set color brush
        QColor c(slope, slope, slope);
        painter.setBrush(c);

        //Create polygon
        QPolygonF triangle;
        triangle.append(QPointF(p1.x(), p1.y()));
        triangle.append(QPointF(p2.x(), p2.y()));
        triangle.append(QPointF(p3.x(), p3.y()));

        painter.drawPolygon(triangle);
    }

    //Draw aspect
    for (Triangle t : aspect_dtm)
    {
        //Get triangle vertices
        QPoint3D p1 = t.getP1();
        QPoint3D p2 = t.getP2();
        QPoint3D p3 = t.getP3();
        double aspect = t.getAspect();

        //Set color brush
        if (aspect >= 345 || aspect < 15)
        {
            QColor d(255, 0, 0);
            painter.setBrush(d);
        }
        else if (aspect >= 15 && aspect < 45)
        {
            QColor d(255, 127, 0);
            painter.setBrush(d);
        }
        else if (aspect >= 45 && aspect < 75)
        {
            QColor d(255, 255, 0);
            painter.setBrush(d);
        }
        else if (aspect >= 75 && aspect < 105)
        {
            QColor d(127, 255, 0);
            painter.setBrush(d);
        }
        else if (aspect >= 105 && aspect < 135)
        {
            QColor d(0, 255, 0);
            painter.setBrush(d);
        }
        else if (aspect >= 135 && aspect < 165)
        {
            QColor d(0, 255, 127);
            painter.setBrush(d);
        }
        else if (aspect >= 165 && aspect < 195)
        {
            QColor d(0, 255, 255);
            painter.setBrush(d);
        }
        else if (aspect >= 195 && aspect < 225)
        {
            QColor d(0, 127, 255);
            painter.setBrush(d);
        }
        else if (aspect >= 225 && aspect < 255)
        {
            QColor d(0, 0, 255);
            painter.setBrush(d);
        }
        else if (aspect >= 255 && aspect < 285)
        {
            QColor d(127, 0, 255);
            painter.setBrush(d);
        }
        else if (aspect >= 285 && aspect < 315)
        {
            QColor d(255, 0, 255);
            painter.setBrush(d);
        }
        else if (aspect >= 315 && aspect < 345)
        {
            QColor d(255, 0, 127);
            painter.setBrush(d);
        }

        //Create polygon
        QPolygonF triangle_asp;
        triangle_asp.append(QPointF(p1.x(), p1.y()));
        triangle_asp.append(QPointF(p2.x(), p2.y()));
        triangle_asp.append(QPointF(p3.x(), p3.y()));

        painter.drawPolygon(triangle_asp);

    }
        painter.end();
}


void Draw::mousePressEvent(QMouseEvent *event)
{
    //Get coordinates of cursor
    QPoint3D p;
    p.setX(event ->x());
    p.setY(event ->y());
    double random = std::rand() * 200.0 / RAND_MAX;
    p.setZ(random);

    //Add point to the list
    points.push_back(p);

    repaint();
};

    void Draw::loadFile(std::string &path, std::vector<QPoint3D> &points,  QSizeF &canvas_size, double &min_z, double &max_z)
    {

        double x, y, z;
        QPoint3D p;

        //Find minimum and maximum x,y,z
        double min_x = std::numeric_limits<double>::max();
        double min_y = std::numeric_limits<double>::max();
        double max_x = std::numeric_limits<double>::min();
        double max_y = std::numeric_limits<double>::min();
        min_z = std::numeric_limits<double>::max();
        max_z = std::numeric_limits<double>::min();

        std::ifstream myfile(path);
        if(myfile.is_open())
        {
            while(myfile >> x >> y >> z)        //read file line by line
            {
                p.setX(x);
                p.setY(y);
                p.setZ(z);

                points.push_back(p);

                if(x < min_x) min_x = x;
                if(x > max_x) max_x = x;
                if(y < min_y) min_y = y;
                if(y > max_y) max_y = y;
                if(z < min_z) min_z = z;
                if(z > max_z) max_z = z;
            }

            myfile.close();
        }

        //Scale points to canvas size
            double h = canvas_size.height() - 50;
            double w = canvas_size.width() - 50;

            double x_coef = w/(max_x-min_x);
            double y_coef = h/(max_y-min_y);

            for(unsigned int i = 0; i < points.size(); i++)
            {
                points[i].setX((points[i].x()-min_x)*x_coef);
                points[i].setY((points[i].y()-min_y)*y_coef);
            }
    }
    /*
    points.clear();
    repaint();
    double x;
    double y;
    double z;

    //Load data from .txt file
    std::ifstream data(path);

    if(data.is_open())
    {
        //Fill the vectors
        while(data >> x >> y >> z)
        {
            QPoint3D point;
            point.setX(x);
            point.setY(y);
            point.setZ(z);
            points.push_back(point);
        }

        data.close();
    }
    repaint();*/



