#ifndef DRAW_H
#define DRAW_H

#include <QWidget>
#include <vector>
#include <QtGui>



class Draw : public QWidget
{
    Q_OBJECT
private:
    boolean draw_mode;
    std::vector <QPoint> polygon;
    std::vector<QPolygon> pol;
    QPoint q;
    int result;
    std::vector<int>res;

public:
    explicit Draw(QWidget *parent = nullptr);

    //signals:
    void setResult(int res);
    void mousePressEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent *e);
    void changeMode(){draw_mode=!draw_mode;}
    std::vector <QPoint> & getPolygon (){return polygon;}
    std::vector <QPolygon> & getPol (){return pol;}
    QPoint & getPoint(){return q;}
    void loadFile(std::string &path);
    void setRes(std::vector<int> ress){res = ress;}
};

#endif // DRAW_H
