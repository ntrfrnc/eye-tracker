#include "plotwidget.h"
#include <QPainter>

PlotWidget::PlotWidget(QWidget *parent) : QWidget(parent) {}

void PlotWidget::paintEvent(QPaintEvent *) {
  QPainter painter(this);
  painter.setBackgroundMode(Qt::TransparentMode);
  QPen pen(QColor(0xFF, 0, 0, 0xAA));
  pen.setCapStyle(Qt::RoundCap);
  pen.setWidth(3);
  painter.setRenderHint(QPainter::Antialiasing, true);
  painter.setPen(pen);
  painter.drawLines(points);
  painter.end();
}

void PlotWidget::addPoint(QPointF point) {
  this->points.append(point);
}

void PlotWidget::clearPoints() {
  this->points.clear();
}
