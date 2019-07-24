#include "LifeWidget.h"

LifeWidget::LifeWidget(QWidget *parent) :
    QWidget(parent)
{
    createBack();
    timer = new QTimer(this);

}

/*! Старт игры с интервалом \a msec */
void LifeWidget::start(int msec)
{
    //заполняем кэш
    for (int i = 0; i < N; ++i) {
        QList<int> x;
        if (i == 0) {
            x.append(N-1);
            x.append(0);
            x.append(1);
        } else if (i == (N-1)) {
            x.append(N-2);
            x.append(N-1);
            x.append(0);
        }  else {
           x.append(i-1);
           x.append(i);
           x.append(i+1);
        }

        cachePoints.insert(i, x);
    }

    qsrand(QDateTime::currentDateTime().toMSecsSinceEpoch());
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
             current[i][j] = (qrand() % 2);
        }
    }

    timer->disconnect();
    connect(timer, &QTimer::timeout, [=](){
        life();

        this->update();
    });
    timer->start(msec);
}

/*! Стоп игры */
void LifeWidget::stop()
{
    timer->stop();
}

/*! Генерация следующей итерации "жизни" */
void LifeWidget::life()
{
    if (cachePoints.count() < N)
        return;

    //расчёт каждой точки в карте
    for (int i = 0; i < N; ++i) {
        QList<int> x = cachePoints.value(i);

        for (int j = 0; j < N; ++j) {

            quint8 count = 0;

            QList<int> y = cachePoints.value(j);

            for (int a = 0; a < 3; ++a) {
                for (int b = 0; b < 3; ++b) {
                    if (a == 1 && b == 1)
                        continue;
                    if (current[x.at(a)][y.at(b)])
                        count++;
                }
            }

            if (current[i][j]) { //если есть жизнь, смотрим кол-во соседей
                if (count == 2 || count == 3)
                    next[i][j] = true;  //продолжает жить
                else
                    next[i][j] = false; //умирает
            } else {
                if (count == 3)
                    next[i][j] = true;  //начинает жить
                else
                    next[i][j] = false; //жизнь так и не зародилась
            }

        }
    }

    //копируем массивы
    for (int i = 0; i < N; ++i) {
        memcpy(current[i], next[i], N * sizeof(bool));
    }
}

/*! Создание заднего фона */
void LifeWidget::createBack()
{
    int w = this->width() / N;
    int h = this->height() / N;

    imageBack = std::make_shared<QImage>(width(), height(), QImage::Format_ARGB32);

    if (w == 0)
        w = 1;
    if (h == 0)
        h = 1;

    QPainter painter;
    painter.begin(imageBack.get());

    painter.setPen(Qt::DotLine);
    painter.setBrush(Qt::NoBrush);

    for (int i = 0; i < this->width(); i=i+w) {
        painter.drawLine(i, 0, i, this->height());
    }
    for (int i = 0; i < this->height(); i=i+h) {
        painter.drawLine(0, i, this->width(), i);
    }
    painter.end();
}

/*! Отрисовка на виджете */
void LifeWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    int w = this->width() / N;
    int h = this->height() / N;

    painter.setPen(Qt::NoPen);
    QBrush br1 = QBrush(QColor(Qt::darkGreen));
    QBrush brW = QBrush(QColor(Qt::white));

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (current[i][j]) {
                painter.setBrush(br1);
            } else {
                painter.setBrush(brW);
            }
            painter.drawRect(i * w, j * h, w, h);
        }
    }

    painter.save();
    //отрисовываем задний фон
    painter.drawImage(this->rect(), *imageBack.get());
    painter.restore();
}

/*! При изменении окна - изменяем задний фон */
void LifeWidget::resizeEvent(QResizeEvent *)
{
    createBack();
}
