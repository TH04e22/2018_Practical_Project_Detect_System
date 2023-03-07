#ifndef SCALEDCHART_H
#define SCALEDCHART_H


#include <QtCharts/QChart>

QT_BEGIN_NAMESPACE
class QGestureEvent;
QT_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

//![1]
class ScaledChart : public QChart
//![1]
{
public:
    explicit ScaledChart(QGraphicsItem *parent = 0, Qt::WindowFlags wFlags = 0);
    ~ScaledChart();

protected:
    //bool sceneEvent(QEvent *event);

private:
    //bool gestureEvent(QGestureEvent *event);

private:

};

#endif // SCALEDCHART_H
