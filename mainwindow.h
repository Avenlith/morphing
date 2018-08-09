#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void czysc(QImage *img);
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void rysujLinie(double x0, double y0, double x1, double y1, QImage *img);
    void zapalPiksel(unsigned char *ptr, int x, int y);
    void rysujPkt(double x0, double y0, QImage *img);
    void generujSiatke(QImage *img, int pkt_img[2][66]);
    void ustawPktDomyslnie(int pkt_img[2][66]);
    void czyTrafil(int x0, int y0, int pkt_img[2][66]);
    void przepiszObraz(QImage *img, QImage *img_2);
    void wyznaczPktTr(int pktTr[2][3][100], int pkt_img[2][66]);
    void teksturowanie(int n, int tmp_pkt_tr[2][3]);
    void morphing();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;

    QImage *img0, *img1, *tmp_img0, *tmp_img1, *img2, *tr0, *tr1;
    int szer;
    int wys;
    int poczX0, poczX1, poczX2;
    int poczY0, poczY1, poczY2;
    int pkt_img0[2][66];
    int pkt_img1[2][66];
    int pkt_tr0[2][3][100];
    int pkt_tr1[2][3][100];
    int tmp0_pkt_tr[0][3];
    int tmp1_pkt_tr[0][3];
    int n = 66;
    int NN = 1;
    int ktory = 0;
    bool czyPrzezRuch = false;
    int czyTrafil_n = -1;
};

#endif // MAINWINDOW_H
