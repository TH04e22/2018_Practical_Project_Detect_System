#include "scaledchartview.h"

#include <QtGui/QMouseEvent>

ScaledChartView::ScaledChartView(QChart *chart, QWidget *parent) :
    QChartView(chart, parent),
    m_isTouching(false)
{
    // setRubberBand(QChartView::RectangleRubberBand);
}

bool ScaledChartView::viewportEvent(QEvent *event)
{
    if (event->type() == QEvent::TouchBegin) {
        // By default touch events are converted to mouse events. So
        // after this event we will get a mouse event also but we want
        // to handle touch events as gestures only. So we need this safeguard
        // to block mouse events that are actually generated from touch.
        m_isTouching = true;

        // Turn off animations when handling gestures they
        // will only slow us down.
        chart()->setAnimationOptions(QChart::NoAnimation);
    }
    return QChartView::viewportEvent(event);
}

void ScaledChartView::mousePressEvent(QMouseEvent *event)
{
    if (m_isTouching)
        return;
    shift = event->x();
    QChartView::mousePressEvent(event);
}

void ScaledChartView::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isTouching)
        return;
    QChartView::mouseMoveEvent(event);
}

void ScaledChartView::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_isTouching)
        m_isTouching = false;

    // Because we disabled animations when touch event was detected
    // we must put them back on.
    shift = shift - event->x();
    chart()->scroll(shift, 0);
    chart()->setAnimationOptions(QChart::SeriesAnimations);

    QChartView::mouseReleaseEvent(event);
}


void ScaledChartView::wheelEvent(QWheelEvent *event)
{
    chart()->zoomReset();

    mFactor *= event->angleDelta().y() > 0 ? 2 : 0.5;

    QRectF rect = chart()->plotArea();
    QPointF c = chart()->plotArea().center();
    rect.setWidth(mFactor*rect.width());
    rect.moveCenter(c);
    chart()->zoomIn(rect);

    QChartView::wheelEvent(event);
};

void ScaledChartView::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Left:
        chart()->scroll(-10, 0);
        break;
    case Qt::Key_Right:
        chart()->scroll(10, 0);
        break;
    default:
        QGraphicsView::keyPressEvent(event);
        break;
    }
};
