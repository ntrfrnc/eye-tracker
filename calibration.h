#ifndef CALIBRATION_H
#define CALIBRATION_H

#include <QObject>
#include <QPointF>

class Calibration : public QObject {
  Q_OBJECT

  uint cPointsQuantity;
  uint screenWidth;
  uint screenHeight;
  QPointF *sPs; // screen calibration points
  QPointF *ePs; // eyetracker calibration points
  qreal ax;
  qreal ay;
  qreal bx;
  qreal by;
  qreal cx;
  qreal cy;
  bool calculated;

 public:
  explicit Calibration(QObject *parent = 0);
  explicit Calibration(uint screenWidth, uint screenHeight);
  void setScreenSize(uint width, uint height);
  void updateScreenPoints();
  bool calculateFactors(QPointF *ePs);
  bool isCalculated();
  QPointF getPointOnScreen(QPointF point);
  uint getScreenWidth();
  uint getScreenHeight();

 signals:

 public slots:
};

#endif  // CALIBRATION_H
