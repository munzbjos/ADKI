#include "widget.h"
#include "ui_widget.h"
#include "algorithms.h"
#include "draw.h"
#include <QElapsedTimer>
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

        //Create convex hull
        QPolygon ch;
        Algorithms alg;
        QElapsedTimer timer;
        std::vector<QPoint> points = ui->Canvas->getPoints();
        timer.start();
        //Create CH
        if (ui->comboBox->currentIndex() == 0)
            ch = alg.jarvis(points);
        else if(ui->comboBox->currentIndex() == 1)
            ch = alg.qHull(points);
        else if(ui->comboBox->currentIndex() == 2)
            ch = alg.sweepLine(points);
        else
            ch = alg.graham(points);

        if (ui->checkBox->isChecked())
        {
            QPolygon b = alg.strictlyConvexHull(ch);
            std::vector<QPoint> strictpoints;

            // Points in strict Convex Hull
            for (int i = 0; i < b.size(); i++)
            {
                strictpoints.push_back(b[i]);
            }

            ui->Canvas->setCH(b);
            ui->Canvas->setStrictPoints(strictpoints);
            ui->label_2->setText(QString::number(timer.elapsed()) + " ms");
            repaint();
        }
        else
        //Draw
        ui->label_2->setText(QString::number(timer.elapsed()) + " ms");
        ui->Canvas->setCH(ch);
        repaint();

}



void Widget::on_pushButton_2_clicked()
{
    ui->Canvas->clearCH();
}

void Widget::on_pushButton_3_clicked()
{
    ui->Canvas->clearCH();
    ui->Canvas->clearPoints();
    ui->Canvas->clearStrictPoints();

}

void Widget::on_pushButton_4_clicked()
{
    QSize size = ui->Canvas->size();
    int height = size.height();
    int width = size.width();
    std::vector <QPoint> poly;
    int number = ui->lineEdit->text().toInt();
    if (ui->comboBox_2->currentIndex() == 0)
        poly = ui->Canvas->generatePoints(number, height, width);
    if (ui->comboBox_2->currentIndex() == 1)
        poly = ui->Canvas->generateGrid(number, height, width);
    if (ui->comboBox_2->currentIndex() == 2)
        poly = ui->Canvas->generateCircle(number, height, width);
    if (ui->comboBox_2->currentIndex() == 3)
        poly = ui->Canvas->generateSquare(number, height);
    if (ui->comboBox_2->currentIndex() == 4)
        poly = ui->Canvas->generateEllipse(number, height, width);



    ui->Canvas->setPoints(poly);
    ui->Canvas->clearCH();
    ui->Canvas->repaint();
}
