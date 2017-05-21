#ifndef EYEPOINTERWIDGET_H
#define EYEPOINTERWIDGET_H

#include "calibration.h"

#include <QPointF>
#include <QWidget>

class EyePointerWidget : public QWidget {
  Q_OBJECT

  QPointF point;
  Calibration *calibration;

  void paintEvent(QPaintEvent *);

 public:
  explicit EyePointerWidget(QWidget *parent = 0);
  void setCalibration(Calibration *calibration);

 signals:

 public slots:
  void setPoint(QPointF point);
  void setEyesPosition(QPointF point);
};

#endif  // EYEPOINTERWIDGET_H
