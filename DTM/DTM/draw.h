#ifndef DRAW_H
#define DRAW_H

#include <QWidget>
#include <vector>
#include <fstream>

#include "qpoint3d.h"
#include "edge.h"
#include "triangle.h"


class Draw : public QWidget
{
    Q_OBJECT
private:
    std::vector<QPoint3D> points;
    std::vector <Edge> dt;
    std::vector<Edge> contours;
    double z_min, z_max, dz;
    std::vector <Triangle> dtm, aspect_dtm;


public:
    explicit Draw(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void setPoints(std::vector<QPoint3D> &points_){points=points_;}
    std::vector<QPoint3D> & getPoints(){return points;}
    void setDT(std::vector<Edge> &dt_){dt = dt_;}
    std::vector<Edge> & getDT(){return dt;}
    void setContours(std::vector<Edge> &contours_){contours=contours_;}
    std::vector<Edge>& getContours() {return contours;}
    void loadFile(std::string &path, std::vector<QPoint3D> &points,  QSizeF &canvas_size, double &min_z, double &max_z);
    void setDTM(std::vector<Triangle> &dtm_){dtm=dtm_;}
    std::vector<Triangle> & getDTM(){return dtm;}
    void setAspectDTM(std::vector<Triangle> &aspect_dtm_){aspect_dtm = aspect_dtm_;}
    void clearAspect(){aspect_dtm.clear();}

signals:

public slots:
};

#endif // DRAW_H
