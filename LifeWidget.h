#ifndef LIFEWIDGET_H
#define LIFEWIDGET_H

#include <QDateTime>
#include <QMap>
#include <QPainter>
#include <QTimer>
#include <QWidget>

#include <memory>

class LifeWidget : public QWidget
{
public:
    explicit LifeWidget(QWidget *parent = 0);

    void start(int msec);
    void stop();

protected:
    void life();
    void createBack();
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);

private:
    static constexpr int N = 70;            //!< Размер

    bool current[N][N];                     //!< Текущая жизнь
    bool next[N][N];                        //!< Массив, куда идут расчёты

    std::shared_ptr<QImage> imageBack;      //!< Задний фон
    QTimer * timer;                         //!< Таймер генерации жизни

    QMap<int, QList<int>> cachePoints;      //!< Кэш точек
};

#endif // LIFEWIDGET_H
