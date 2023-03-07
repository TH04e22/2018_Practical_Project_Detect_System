#ifndef SCALEDCHARTVIEW_H
#define SCALEDCHARTVIEW_H


#include <QtCharts/QChartView>
#include <QtWidgets/QRubberBand>

QT_CHARTS_USE_NAMESPACE

//![1]
class ScaledChartView : public QChartView
//![1]
{
public:
    ScaledChartView(QChart *chart, QWidget *parent = 0);

//![2]
protected:
    bool viewportEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);
//![2]

private:
    bool m_isTouching;
    qreal mFactor=1.0;
    int shift;
};

#endif // SCALEDCHARTVIEW_H
