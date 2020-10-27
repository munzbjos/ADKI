#include "widget.h"
#include "ui_widget.h"
#include "algorithms.h"
#include <iostream>
#include <QFileDialog>
#include <QFile>
#include <fstream>
#include <QMessageBox>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_clicked()
{
    //change mode
    ui->Canvas->changeMode();
}

void Widget::on_pushButton_3_clicked()
{
    //Get reference to points
    std::vector<QPoint> &canvas_polygon = ui->Canvas->getPolygon();
    std::vector<QPolygon> &canvas_pol = ui->Canvas->getPol();
    QPoint &q = ui->Canvas->getPoint();
    q.setX(-50);
    q.setY(-50);
    //Delete all points
    canvas_polygon.clear();
    canvas_pol.clear();
    ui->Canvas->setRes(std::vector<int>());



    //Repaint screen
    repaint();
}

void Widget::on_pushButton_2_clicked()
{
    {
        //Analyze point and polygon position
        QPoint q = ui->Canvas->getPoint();
        std::vector<QPoint> polygon = ui->Canvas->getPolygon();
        std::vector<QPolygon> pol = ui->Canvas->getPol();
        QPolygon one;
        std::vector<int> ressult;
        int p = 0;
        int result = 0;
        int ress = 0;
        int maxi = 0;
        Algorithms alg;
        if (pol.empty()==false)
        {
            //Get one polygon
            for (unsigned int j = 0; j < pol.size(); j++) {
                one = pol[j];
                for (int  i= 0; i< one.size(); i++) {

                    polygon.push_back(one[i]);

                }

                for (unsigned int k = 0; k < polygon.size(); k++) {
                    if (q == polygon[k])
                        p += 1;
                }

                //Check if point q is on vertex
                if (p < 1)
                {

                    //Select method
                    if (ui->comboBox->currentIndex() == 0)
                        ress = alg.getPositionWinding(q, polygon);
                    else
                    {
                        ress = alg.getPositionRay(q, polygon);
                    }
                    if (ui->comboBox->currentIndex() == 0)
                    {
                        maxi = alg.getPositionWinding(q, polygon);
                        if (maxi>result)
                        {
                            result = maxi;
                        }
                    }
                    else
                    {
                        maxi = alg.getPositionRay(q, polygon);
                        if (maxi>result)
                            result = maxi;
                    }
                }
                ressult.push_back(ress);
                polygon.clear();
            }


            p=0;
        }
        if (pol.empty() == true)
        {
            if (ui->comboBox->currentIndex() == 0)
                result = alg.getPositionWinding(q, polygon);
            else
                result = alg.getPositionRay(q, polygon);
        }


        //Print results
        if (result == 1)
            ui->label->setText("Inside");
        else if (result == 2)
            ui->label->setText("Border");
        else if (result == 3)
            ui->label->setText("Vertex");
        else
            ui->label->setText("Outside");
        ui->Canvas->setResult(result);
        ui->Canvas->setRes(ressult);
        ui->Canvas->repaint();

    }
}

void Widget::on_pushButton_4_clicked()
{
    QString path(QFileDialog::getOpenFileName(this, tr("Open file with polygons"), "../", tr("Text Files (*.txt)")));
    std::string path_std = path.toStdString();
    ui->Canvas->loadFile(path_std);
}

void Widget::on_pushButton_5_clicked()
{
    int number = ui->spinBox->value();
    ui->Canvas->getPolygon().clear();
    ui->Canvas->getPol().clear();

    //Input cannot be a triangle, line or point
    if(number < 4){
        QMessageBox msgBox;
        msgBox.setText("Warning: The input has to be at least 4");
        msgBox.exec();
        return;
    }

    for (int i = 0; i < number; i++)
    {
        double x = 0.0 + (rand() % static_cast<int>(650.0 - 0.0 + 1));
        double y = 0.0 + (rand() % static_cast<int>(650.0 - 0.0 + 1));

        ui->Canvas->getPolygon().push_back(QPoint(x,y));

    }
    ui->Canvas->repaint();

}
