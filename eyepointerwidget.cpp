#include "eyepointerwidget.h"
#include <QPainter>
#include <QPointF>

EyePointerWidget::EyePointerWidget(QWidget *parent) : QWidget(parent) {}

void EyePointerWidget::setCalibration(Calibration *calibration){
  this->calibration = calibration;
}

void EyePointerWidget::paintEvent(QPaintEvent *) {
  QPainter painter(this);
  painter.setBackgroundMode(Qt::TransparentMode);
  QPen pen(QColor(0xFF, 0, 0, 0xAA));
  pen.setCapStyle(Qt::RoundCap);
  pen.setWidth(30);
  painter.setRenderHint(QPainter::Antialiasing, true);
  painter.setPen(pen);
  painter.drawPoint(point);
}

void EyePointerWidget::setPoint(QPointF point) {
  this->point = point;
  this->repaint();
}

void EyePointerWidget::setEyesPosition(QPointF point) {
  QPointF sPoint = calibration->getPointOnScreen(point);
  sPoint.setY(calibration->getScreenHeight() - sPoint.y());
  this->point = sPoint;
  this->repaint();
}
