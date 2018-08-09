#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Morphing - Mateusz Foks");
    //2 obrazki mają taki sam rozmiar
    szer = ui->frame0->width();
    wys = ui->frame0->height();

    poczX0 = ui->frame0->x();
    poczY0 = ui->frame0->y();
    poczX1 = ui->frame1->x();
    poczY1 = ui->frame1->y();
    poczX2 = ui->frame2->x();
    poczY2 = ui->frame2->y();


    img0 = new QImage(szer,wys,QImage::Format_RGB32);
    img1 = new QImage(szer,wys,QImage::Format_RGB32);
    img2 = new QImage(szer,wys,QImage::Format_RGB32);

    tr0 = new QImage(szer,wys,QImage::Format_RGB32);
    tr1 = new QImage(szer,wys,QImage::Format_RGB32);

    tmp_img0 = new QImage(":/rose.jpg");
    tmp_img1 = new QImage(":/rick.jpg");

    ustawPktDomyslnie(pkt_img0);
    ustawPktDomyslnie(pkt_img1);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent*)
{
    // Obiekt klasy QPainter pozwala nam rysowac na komponentach
    QPainter p(this);

    // Rysuje obrazek "img" w punkcie (poczX,poczY)
    // (tu bedzie lewy gorny naroznik)
    //p.drawImage(poczX_kolor, poczY_kolor, *kolor_img);

    p.drawImage(poczX0, poczY0, *img0);
    p.drawImage(poczX1, poczY1, *img1);
    p.drawImage(poczX2, poczY2, *img2);


}

void MainWindow::czysc(QImage *img)
{
    // Wskaznik za pomoca, ktorego bedziemy modyfikowac obraz
    unsigned char *ptr;
    // Funkcja "bits()" zwraca wskaznik do pierwszego piksela danych
    ptr = img->bits();
    int i,j;
    // Przechodzimy po wszystkich wierszach obrazu
    for(i=0; i<wys; i++)
    {
        // Przechodzimy po pikselach danego wiersza
        // W kazdym wierszu jest "szer" pikseli (tzn. 4 * "szer" bajtow)
        for(j=0; j<szer; j++)
        {
            ptr[szer*4*i + 4*j    ] = 0; // Skladowa BLUE
            ptr[szer*4*i + 4*j + 1] = 0; // Skladowa GREEN
            ptr[szer*4*i + 4*j + 2] = 0; // Skladowa RED
        }
    }

    update();
}

void MainWindow::przepiszObraz(QImage *img, QImage *img_2)
{
    // Wskaznik za pomoca, ktorego bedziemy modyfikowac obraz
    unsigned char *ptr, *ptr_2;
    // Funkcja "bits()" zwraca wskaznik do pierwszego piksela danych
    ptr = img->bits();
    ptr_2 = img_2->bits();
    int i,j;
    // Przechodzimy po wszystkich wierszach obrazu
    for(i=0; i<wys; i++)
    {
        // Przechodzimy po pikselach danego wiersza
        // W kazdym wierszu jest "szer" pikseli (tzn. 4 * "szer" bajtow)
        for(j=0; j<szer; j++)
        {
            ptr_2[szer*4*i + 4*j    ] = ptr[szer*4*i + 4*j    ];
            ptr_2[szer*4*i + 4*j + 1] = ptr[szer*4*i + 4*j + 1];
            ptr_2[szer*4*i + 4*j + 2] = ptr[szer*4*i + 4*j + 2];
        }
    }

    update();
}

void MainWindow::czyTrafil(int x0, int y0, int pkt_img[2][66])
{
    czyTrafil_n = -1;

    for(int k = 0; k < n; k++)
    {
        if(x0 >= pkt_img[0][k] - 3 && x0 <= pkt_img[0][k] + 3 && y0 >= pkt_img[1][k] - 3 && y0 <= pkt_img[1][k] + 3)
        {
            czyTrafil_n = k;
            break;
        }
    }

}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    czyPrzezRuch = false;
    int x0 = event->x();
    int y0 = event->y();
    int x1, y1;
    ktory = 0;
    if(x0 >= poczX0 && x0 <= poczX0+szer && y0 >= poczY0 && y0 <= poczY0+wys)
    {
        ktory = 1;

        x1 = x0 - poczX0;
        y1 = y0 - poczY0;
        czyTrafil(x1, y1, pkt_img0);

    }
    else if(x0 >= poczX1 && x0 <= poczX1+szer && y0 >= poczY1 && y0 <= poczY1+wys)
    {
        ktory = 2;

        x1 = x0 - poczX1;
        y1 = y0 - poczY1;
        czyTrafil(x1, y1, pkt_img1);
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    czyPrzezRuch = true;
    int x0 = event->x();
    int y0 = event->y();
    int x1, y1;
    if(czyTrafil_n != -1)
    {
        if(ktory == 1)
        {
            przepiszObraz(tmp_img0, img0);
            x1 = x0 - poczX0;
            y1 = y0 - poczY0;
            pkt_img0[0][czyTrafil_n] = x1;
            pkt_img0[1][czyTrafil_n] = y1;
            generujSiatke(img0, pkt_img0);
        }
        else if(ktory == 2)
        {
            przepiszObraz(tmp_img1, img1);
            x1 = x0 - poczX1;
            y1 = y0 - poczY1;
            pkt_img1[0][czyTrafil_n] = x1;
            pkt_img1[1][czyTrafil_n] = y1;
            generujSiatke(img1, pkt_img1);
        }
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    czyPrzezRuch = false;
}

void MainWindow::rysujLinie(double x0, double y0, double x1, double y1, QImage *img)
{
    // Wskaznik za pomoca, ktorego bedziemy modyfikowac obraz
    unsigned char *ptr;
    // Funkcja "bits()" zwraca wskaznik do pierwszego piksela danych
    ptr = img->bits();
    double y, z, a, b;
    if((x1 - x0) > 0) // rysowana linia od lewej do prawej
    {
        a = (y1-y0)/(x1-x0);
        b = y1 - a*x1;
        for(double x = x0; x < x1; ++x)
        {
            y = a*x + b;
            z = a*(x+1) + b; //obliczanie funkcji w przód aby sprawdzić czy nie ma za dużego przeskoku aby wypełnić pustą przestrzeń
            //zapalPiksel(ptr, x, y);
            //if((y - z) >= 0)
            for(int k = (int)floor(z+0.5); k <= (int)floor(y + 0.5) ; ++k)
                zapalPiksel(ptr, x, k);

            for(int k = (int)floor(y+0.5); k <= (int)floor(z + 0.5) ; ++k)
                zapalPiksel(ptr, x, k);

        }
    }
    else if((x1 - x0) < 0) //rysowana linia od prawej do lewej
    {
        a = (y0-y1)/(x0-x1);
        b = y0 - a*x0;
        for(int x = x1; x < x0; ++x)
        {
            y = a*x + b;
            z = a*(x+1) + b;
            //czy rysowana od gory do dołu lub na odwrót - poprawa dokladnosci przy duzych katach

            for(int k = (int)floor(z+0.5); k <= (int)floor(y + 0.5); ++k)
                zapalPiksel(ptr, x, k);

            for(int k = (int)floor(y+0.5); k <= (int)floor(z + 0.5); ++k)
                zapalPiksel(ptr, x, k);
        }
    }
    else //dla odcinkow pionowych
    {
        // czy od gory do dolu
        for(int y=y0; y <= y1; ++y)
            zapalPiksel(ptr, (int)floor(x0+0.5), y);
        for(int y=y0; y >= y1; --y)
            zapalPiksel(ptr, (int)floor(x0+0.5), y);
    }
    update();
}

void MainWindow::zapalPiksel(unsigned char *ptr, int x, int y)
{
    if(szer - x > 0 && szer + x >= szer && wys - y > 0 && wys + y >= wys)
    {
        ptr[szer*4*y + 4*x] = 0; // Skladowa BLUE
        ptr[szer*4*y + 4*x + 1] = 0; // Skladowa GREEN
        ptr[szer*4*y + 4*x + 2] = 255; // Skladowa RED
    }
}

void MainWindow::rysujPkt(double x, double y, QImage *img)
{
    unsigned char *ptr;
    ptr = img->bits();
    for(int i = y - 6; i < y + 6; i++)
    {
        for(int j = x - 6; j < x + 6; j++)
        {
            zapalPiksel(ptr, j, i);
        }
    }
}

void MainWindow::ustawPktDomyslnie(int pkt_img[2][66])
{
    int n = 0;
    for(int i = 0; i <= wys; i+=100)
    {
        for(int j = 0; j <= szer; j+=50)
        {
            pkt_img[0][n] = i;
            pkt_img[1][n] = j;
            n++;
        }
        //k++;
    }
    //qDebug() << k;
}

void MainWindow::wyznaczPktTr(int pktTr[2][3][100], int pkt_img[2][66])
{
    int n = 1;
    int k = 11;
    for(int i = 1; n <= 100; i++)
    {
        if(i % 11 != 0)
        {
            i--;

            pktTr[0][0][n-1] = pkt_img[0][i];
            pktTr[1][0][n-1] = pkt_img[1][i];

            pktTr[0][1][n-1] = pkt_img[0][i + k];
            pktTr[1][1][n-1] = pkt_img[1][i + k];

            pktTr[0][2][n-1] = pkt_img[0][i + 1];
            pktTr[1][2][n-1] = pkt_img[1][i + 1];

            n++;

            pktTr[0][0][n-1] = pkt_img[0][i + 1];
            pktTr[1][0][n-1] = pkt_img[1][i + 1];

            pktTr[0][1][n-1] = pkt_img[0][i + k];
            pktTr[1][1][n-1] = pkt_img[1][i + k];

            pktTr[0][2][n-1] = pkt_img[0][i + k + 1];
            pktTr[1][2][n-1] = pkt_img[1][i + k + 1];

            n++;
            i++;

        }
    }
}

void MainWindow::generujSiatke(QImage *img, int pkt_img[2][66])
{
    int k = 11;
    for(int i = 0; i < n; i++)
    {
        rysujPkt(pkt_img[0][i], pkt_img[1][i], img);
        if(i >= 1 && i % 11 != 0)
        {
            rysujLinie(pkt_img[0][i-1], pkt_img[1][i-1], pkt_img[0][i], pkt_img[1][i], img);
        }
        if(i >= 1 && i % k == 0)
        {
            rysujLinie(pkt_img[0][i-11], pkt_img[1][i-11], pkt_img[0][i], pkt_img[1][i], img);
            k++;
        }
        if(i >= 11 && (i % 11) <= 9)
        {
            rysujLinie(pkt_img[0][i-10], pkt_img[1][i-10], pkt_img[0][i], pkt_img[1][i], img);
        }
    }
    update();
}

void MainWindow::morphing()
{
    int Ax, Ay, Bx, By, Cx, Cy, tAx, tAy, tBx, tBy, tCx, tCy;
    int n_Ax, n_Ay, n_Bx, n_By, n_Cx, n_Cy;

    przepiszObraz(tmp_img0, img0);
    przepiszObraz(tmp_img1, img1);
    qDebug() << "dziala";
    for(int n = 0; n < NN; n++)
    {
        for(int i = 0; i < 100; i++)
        {
            Ax = pkt_tr0[0][0][i]; Ay = pkt_tr0[1][0][i];
            Bx = pkt_tr0[0][1][i]; By = pkt_tr0[1][1][i];
            Cx = pkt_tr0[0][2][i]; Cy = pkt_tr0[1][2][i];

            tAx = pkt_tr1[0][0][i]; tAy = pkt_tr1[1][0][i];
            tBx = pkt_tr1[0][1][i]; tBy = pkt_tr1[1][1][i];
            tCx = pkt_tr1[0][2][i]; tCy = pkt_tr1[1][2][i];

            tmp1_pkt_tr[0][0] = (1 - ((double)n / (double)NN))*(double)Ax + ((double)n / (double)NN)*(double)tAx;
            tmp1_pkt_tr[1][0] = (1 - ((double)n / (double)NN))*(double)Ay + ((double)n / (double)NN)*(double)tAy;

            tmp1_pkt_tr[0][1] = (1 - ((double)n / (double)NN))*(double)Bx + ((double)n / (double)NN)*(double)tBx;
            tmp1_pkt_tr[1][1] = (1 - ((double)n / (double)NN))*(double)By + ((double)n / (double)NN)*(double)tBy;

            tmp1_pkt_tr[0][2] = (1 - ((double)n / (double)NN))*(double)Cx + ((double)n / (double)NN)*(double)tCx;
            tmp1_pkt_tr[1][2] = (1 - ((double)n / (double)NN))*(double)Cy + ((double)n / (double)NN)*(double)tCy;

            for(int k = 0; k < 3; k++)
            {
                qDebug() <<tmp1_pkt_tr[0][i];
                qDebug() <<tmp1_pkt_tr[1][i];
                qDebug() << "";
            }

            qDebug() << "i: " << i;

            //teksturowanie(n, tmp1_pkt_tr);

        }
    }
}

void MainWindow::teksturowanie(int n, int tmp_pkt_tr[2][3])
{
    double u, v, w, x1, y1, a, b;
    int n1, n2, n3, n4;
    unsigned char *ptr0, *ptr1, *ptr_wynik;
    ptr0 = img0->bits();
    ptr1 = img1->bits();
    ptr_wynik = img2->bits();
    qDebug() << "dziala";
    double Ax0, Bx0, Cx0, Ay0, By0, Cy0, Ax1, Bx1, Cx1, Ay1, By1, Cy1, tAx, tAy, tBx, tBy, tCx, tCy;

    Ax0 = pkt_tr0[0][0][n]; Ay0 = pkt_tr0[1][0][n];
    Bx0 = pkt_tr0[0][1][n]; By0 = pkt_tr0[1][1][n];
    Cx0 = pkt_tr0[0][2][n]; Cy0 = pkt_tr0[1][2][n];

    Ax1 = pkt_tr1[0][0][n]; Ay1 = pkt_tr1[1][0][n];
    Bx1 = pkt_tr1[0][1][n]; By1 = pkt_tr1[1][1][n];
    Cx1 = pkt_tr1[0][2][n]; Cy1 = pkt_tr1[1][2][n];


    tAx = tmp_pkt_tr[0][0]; tAy = tmp_pkt_tr[1][0];
    tBx = tmp_pkt_tr[0][1]; tBy = tmp_pkt_tr[1][1];
    tCx = tmp_pkt_tr[0][2]; tCy = tmp_pkt_tr[1][2];

    for(int i = 0; i < wys; i++)
    {
        for(int j = 0; j < szer; j++)
        {
            v = (((double)j - tAx)*(tCy - tAy) - ((double)i - tAy)*(tCx - tAx)) / ((tBx - tAx)*(tCy - tAy) - (tBy - tAy)*(tCx - tAx));
            w = ((tBx - tAx)*((double)i - tAy) - (tBy - tAy)*((double)j - tAx)) / ((tBx - tAx)*(tCy - tAy) - (tBy - tAy)*(tCx - tAx));
            u = 1 - v - w;
            qDebug() << v << " " << w << " " << u;
            if(v < 0 || v > 1 || w < 0 || w > 1 || u < 0 || u > 1)
            {

            }
            else
            {
                //the first one
                y1 = u * (double)Ax0 + v * (double)Bx0 + w * (double)Cx0;
                x1 = u * (double)Ay0 + v * (double)By0 + w * (double)Cy0;

                n1 = szer*4*(int)(x1) + 4*(int)(y1);
                n2 = szer*4*(int)(x1+1) + 4*(int)(y1+1);
                n4 = szer*4*(int)(x1) + 4*(int)(y1+1);
                n3 = szer*4*(int)(x1+1) + 4*(int)(y1);

                a = x1 - (int)x1;
                b = y1 - (int)y1;

                int kolor_b0 = b * ((1 - a) * ptr0[n1    ] + a * ptr0[n2    ]) + (1 - b) * ((1 - a) * ptr0[n4    ] + a * ptr0[n3    ]);
                int kolor_g0 = b * ((1 - a) * ptr0[n1 + 1] + a * ptr0[n2 + 1]) + (1 - b) * ((1 - a) * ptr0[n4 + 1] + a * ptr0[n3 + 1]);
                int kolor_r0 = b * ((1 - a) * ptr0[n1 + 2] + a * ptr0[n2 + 2]) + (1 - b) * ((1 - a) * ptr0[n4 + 2] + a * ptr0[n3 + 2]);

                //ptr_tr0[szer*4*i  + 4*j    ] = kolor_b; // Skladowa BLUE
                //ptr_tr0[szer*4*i  + 4*j + 1] = kolor_g; // Skladowa GREEN
                //ptr_tr0[szer*4*i  + 4*j + 2] = kolor_r; // Skladowa RED

                //the second one
                y1 = u * (double)Ax1 + v * (double)Bx1 + w * (double)Cx1;
                x1 = u * (double)Ay1 + v * (double)By1 + w * (double)Cy1;

                n1 = szer*4*(int)(x1) + 4*(int)(y1);
                n2 = szer*4*(int)(x1+1) + 4*(int)(y1+1);
                n4 = szer*4*(int)(x1) + 4*(int)(y1+1);
                n3 = szer*4*(int)(x1+1) + 4*(int)(y1);

                a = x1 - (int)x1;
                b = y1 - (int)y1;

                int kolor_b1 = b * ((1 - a) * ptr1[n1    ] + a * ptr1[n2    ]) + (1 - b) * ((1 - a) * ptr1[n4    ] + a * ptr1[n3    ]);
                int kolor_g1 = b * ((1 - a) * ptr1[n1 + 1] + a * ptr1[n2 + 1]) + (1 - b) * ((1 - a) * ptr1[n4 + 1] + a * ptr1[n3 + 1]);
                int kolor_r1 = b * ((1 - a) * ptr1[n1 + 2] + a * ptr1[n2 + 2]) + (1 - b) * ((1 - a) * ptr1[n4 + 2] + a * ptr1[n3 + 2]);

                //ptr_tr1[szer*4*i  + 4*j    ] = kolor_b; // Skladowa BLUE
                //ptr_tr1[szer*4*i  + 4*j + 1] = kolor_g; // Skladowa GREEN
                //ptr_tr1[szer*4*i  + 4*j + 2] = kolor_r; // Skladowa RED

                ptr_wynik[szer*4*i  + 4*j    ] = (kolor_b0 * kolor_b1) / 2;
                ptr_wynik[szer*4*i  + 4*j + 1] = (kolor_g0 * kolor_g1) / 2;
                ptr_wynik[szer*4*i  + 4*j + 2] = (kolor_r0 * kolor_r1) / 2;

            }
        }
    }
    update();
}

void MainWindow::on_pushButton_clicked()
{
    //qDebug() << "hello";

    ustawPktDomyslnie(pkt_img0);
    ustawPktDomyslnie(pkt_img1);

    //czysc(img0);
    //czysc(img1);

    przepiszObraz(tmp_img0, img0);
    przepiszObraz(tmp_img1, img1);

    generujSiatke(img0, pkt_img0);
    generujSiatke(img1, pkt_img1);

    wyznaczPktTr(pkt_tr0, pkt_img0);
    wyznaczPktTr(pkt_tr1, pkt_img1);
//    for(int i = 0; i < 100; i++)
//    {
//        for(int j = 0; j < 3; j++)
//        {
//            qDebug() << "x: " << pkt_tr0[0][j][i];
//            qDebug() << "y: " << pkt_tr0[1][j][i];
//        }

//        qDebug() << "i wynosi: " << i;
//    }

    update();
}

void MainWindow::on_pushButton_2_clicked()
{
    wyznaczPktTr(pkt_tr0, pkt_img0);
    wyznaczPktTr(pkt_tr1, pkt_img1);

    morphing();
}
