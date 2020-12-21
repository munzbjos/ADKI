#include "widget.h"
#include "ui_widget.h"
#include "algorithms.h"
#include <QMessageBox>
#include <QFileDialog>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    z_min = 0, z_max = 500, dz = 10;
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_7_clicked()
{
    //Create contour lines
    std::vector<Edge> dt;
    Algorithms a;

    if(dt.size()==0)
    {

    //Get points
    std::vector<QPoint3D> points = ui->Canvas->getPoints();

    //Create DT

    dt=a.DT(points);

    //Set DT
    ui->Canvas->setDT(dt);
    }

    //Create contour lines
    std::vector<Edge> contours = a.contourLines(dt, z_min, z_max, dz);
    ui->Canvas->setContours(contours);
    //Repaint
    repaint();
}


void Widget::on_pushButton_11_clicked()
{
    //Clear points

    //Get points
    std::vector<QPoint3D> &points = ui->Canvas->getPoints();

    //Clear points
    points.clear();

    //Repaint
    repaint();
}


void Widget::on_pushButton_12_clicked()
{
    //Clear DT


    //Get points
    std::vector<Edge> &dt = ui->Canvas->getDT();
    std::vector<Edge> &contours = ui->Canvas->getContours();
    std::vector<Triangle> &dtm = ui->Canvas->getDTM();

    //Clear points
    dt.clear();
    contours.clear();
    dtm.clear();
    ui->Canvas->clearAspect();

    //Repaint
    repaint();
}

void Widget::on_lineEdit_editingFinished()
{
    z_min = ui->lineEdit->text().toDouble();
}

void Widget::on_lineEdit_2_editingFinished()
{
    z_max = ui->lineEdit_2->text().toDouble();
}

void Widget::on_lineEdit_3_editingFinished
()
{
    dz = ui->lineEdit_3->text().toDouble();
}

void Widget::on_pushButton_clicked()
{
    QString path(QFileDialog::getOpenFileName(this, tr("Open file with points"), "../", tr("Text Files (*.txt)")));
    std::string path_std = path.toStdString();
    std::vector<QPoint3D> points;
    QSizeF canvas_size = ui->Canvas->size();
    Draw u;
    u.loadFile(path_std, points, canvas_size, z_min, z_max);
    ui->Canvas->setPoints(points);
    ui->Canvas->repaint();
}

void Widget::on_pushButton_2_clicked()
{
    //Create contour lines
    std::vector<Edge> dt;
    Algorithms a;

    if(dt.size()==0)
    {

    //Get points
    std::vector<QPoint3D> points = ui->Canvas->getPoints();

    //Create DT

    dt=a.DT(points);

    //Set DT
    ui->Canvas->setDT(dt);
    }
    repaint();
}

void Widget::on_pushButton_3_clicked()
{

        std::vector<Edge> dt;
        Algorithms d;

        //Create triangulation

        std::vector<QPoint3D> points = ui->Canvas->getPoints();
        if (points.empty())
        {
            QMessageBox messageBox;
            messageBox.critical(0,"Error","Insert or generete points first!");
            messageBox.setFixedSize(500,200);
        }
        else
        {
            dt = d.DT(points);
            ui->Canvas->setDT(dt);
            //Analyze DTM
            std::vector<Triangle> dtm = d.analyzeDTM(dt);
            ui->Canvas->setDTM(dtm);
            ui->Canvas->clearAspect();
            repaint();
        }

}

void Widget::on_pushButton_4_clicked()
{
    {
        std::vector<Edge> dt;
        Algorithms d;
        //Create triangulation

        std::vector<QPoint3D> points = ui->Canvas->getPoints();
        if (points.empty())
        {
            QMessageBox messageBox;
            messageBox.critical(0,"Error","Insert or generete points first!");
            messageBox.setFixedSize(500,200);
        }
        else
        {
            dt = d.DT(points);
            ui->Canvas->setDT(dt);
            //Analyze DTM
            std::vector<Triangle> aspect_dtm = d.analyzeDTM(dt);
            ui->Canvas->setAspectDTM(aspect_dtm);
            repaint();
        }
    }
}
