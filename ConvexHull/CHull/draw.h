#ifndef DRAW_H
#define DRAW_H

#include <QWidget>
#include <vector>
#include <QtGui>

class Draw : public QWidget
{
    Q_OBJECT

private:
    QPolygon ch;
    std::vector <QPoint> points;

public:
    explicit Draw(QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent *e);
    std::vector<QPoint> &getPoints(){return points;}
    void setPoints(std::vector<QPoint> &pt){points = pt;}
    QPolygon &getCH(){return ch;}
    void clearCH(){ch.clear(); repaint();}
    void setCH(QPolygon &ch_){ ch = ch_;}
    void clearPoints(){points.clear(); repaint();}
    std::vector <QPoint> generatePoints(int n,int height,int width);
    std::vector <QPoint> generateGrid(int n, int height, int width);
    std::vector <QPoint> generateCircle(int n, int height, int width);
    std::vector <QPoint> generateSquare(int n, int height);
    std::vector <QPoint> generateEllipse(int n, int height, int width);


signals:

};

#endif // DRAW_H
