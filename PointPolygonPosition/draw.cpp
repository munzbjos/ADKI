#include "draw.h"
#include <fstream>
#include <QWidget>

Draw::Draw(QWidget *parent) : QWidget(parent)
{
    draw_mode = false;
    q.setX(-50);
    q.setY(-50);

}

void Draw::mousePressEvent(QMouseEvent *e)

{
    //Cursor position
    double x = e->pos().x();
    double y = e->pos().y();

    //mode = polygon
    if (draw_mode == true) // or (draw_mode) in case of negative (!draw_mode)
    {
        //Create new point
        QPoint p(x,y);
        //Add point to the polygon
        polygon.push_back(p);

    }
    //mode = q
    else
    {
        //change coordinates of q
        q.setX(x);
        q.setY(y);
    }
    repaint();
}

void Draw::setResult(int res)
{
    this->result = res;
}

void Draw::paintEvent(QPaintEvent *e)
{
    //start drawing
    QPainter painter(this);
    painter.begin(this);

    //set color of highlighted polygon
    QBrush brush;
    brush.setColor(Qt::green);
    brush.setStyle(Qt::SolidPattern);
    QPainterPath path;

    //draw drawn polygon
    QPolygon poly;
    for (unsigned int i = 0; i < polygon.size(); i++)
    {
        poly.append(polygon[i]);

    }
    path.addPolygon(poly);
    if (result >0)
    {
        painter.fillPath(path, brush);
    }
    painter.drawPolygon(poly);

    //draw loaded polygons
    for(unsigned int i = 0; i < res.size(); i++)
    {
        if(res[i] >0)
        {
            path.addPolygon(pol[i]);
            painter.fillPath(path, brush);
            painter.drawPolygon(pol[i]);
        }
    }

    for(unsigned int i = 0; i < pol.size(); i++)
    {
        QPolygon polygan = pol[i];
        painter.drawPolygon(polygan);
    }

    //draw q point
    int r = 5;
    painter.drawEllipse(q.x()-r,q.y()-r, 2*r, 2*r);
    //painter.drawPoint(q);
    painter.end();
}

void Draw::loadFile(std::string &path)
{
    polygon.clear();
    repaint();
    double x;
    double y;
    int id;
    QPolygon poly;

    //Load data from .txt file
    std::ifstream data(path);

    if(data.is_open())
    {
        //Fill the vectors
        while(data >>id >> x >> y)
        {
            if (id == 1)
            {
                if (poly.isEmpty() == FALSE)
                {
                    pol.push_back(poly);
                }
                poly.clear();
                poly << QPoint(x, y);
            }
            else
                poly << QPoint(x, y);
        }

        //Add last polygon into vector
        pol.push_back(poly);


        data.close();
    }

    repaint();
}
