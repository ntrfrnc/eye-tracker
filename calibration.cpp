#include "calibration.h"

Calibration::Calibration(QObject *parent) : QObject(parent) {
  cPointsQuantity = 5;
  sPs = new QPointF[cPointsQuantity];
  calculated = false;
}

Calibration::Calibration(uint screenWidth, uint screenHeight) {
  cPointsQuantity = 5;

  this->screenWidth = screenWidth;
  this->screenHeight = screenHeight;

  sPs = new QPointF[cPointsQuantity];
  updateScreenPoints();

  calculated = false;
}

bool Calibration::isCalculated(){
    return calculated;
};

void Calibration::setScreenSize(uint width, uint height) {
  this->screenWidth = width;
  this->screenHeight = height;
  updateScreenPoints();
}

void Calibration::updateScreenPoints() {
  uint edgeOffset = 100;
  sPs[0] = QPointF(edgeOffset, screenHeight - edgeOffset);  // Top left
  sPs[1] = QPointF(screenWidth / 2, screenHeight / 2);      // Center
  sPs[2] = QPointF(screenWidth - edgeOffset, edgeOffset);   // Bottom right
  sPs[3] = QPointF(edgeOffset, edgeOffset);                 // Bottom left
  sPs[4] = QPointF(screenWidth - edgeOffset, screenHeight - edgeOffset);  // Top right
}

bool Calibration::calculateFactors(QPointF *ePs) {
  this->ePs = ePs;

  qreal n = cPointsQuantity;

  qreal a = 0;
  qreal b = 0;
  qreal c = 0;
  qreal d = 0;
  qreal e = 0;

  qreal X1 = 0;
  qreal X2 = 0;
  qreal X3 = 0;
  qreal Y1 = 0;
  qreal Y2 = 0;
  qreal Y3 = 0;

  qreal d0;
  qreal dX1;
  qreal dX2;
  qreal dX3;
  qreal dY1;
  qreal dY2;
  qreal dY3;


  for (uint i=0; i < cPointsQuantity; i++) {
    a += ePs[i].x() * ePs[i].x();
    b += ePs[i].y() * ePs[i].y();
    c += ePs[i].x() * ePs[i].y();
    d += ePs[i].x();
    e += ePs[i].y();

    X1 += ePs[i].x() * sPs[i].x();
    X2 += ePs[i].y() * sPs[i].x();
    X3 += sPs[i].x();
    Y1 += ePs[i].x() * sPs[i].y();
    Y2 += ePs[i].y() * sPs[i].y();
    Y3 += sPs[i].y();
  }

  d0 = n*(a*b-c*c)+2*c*d*e-a*e*e-b*d*d;
  dX1 = n*(X1*b-X2*c)+e*(X2*d-X1*e)+X3*(c*e-b*d);
  dX2 = n*(X2*a-X1*c)+d*(X1*e-X2*d)+X3*(c*d-a*e);
  dX3 = X3*(a*b-c*c)+X1*(c*e-b*d)+X2*(c*d-a*e);
  dY1 = n*(Y1*b-Y2*c)+e*(Y2*d-Y1*e)+Y3*(c*e-b*d);
  dY2 = n*(Y2*a-Y1*c)+d*(Y1*e-Y2*d)+Y3*(c*d-a*e);
  dY3 = Y3*(a*b-c*c)+Y1*(c*e-b*d)+Y2*(c*d-a*e);

  if(d0 == 0.0){
      return false;
  }

  ax = dX1/d0;
  bx = dX2/d0;
  cx = dX3/d0;

  ay = dY1/d0;
  by = dY2/d0;
  cy = dY3/d0;

  calculated = true;

  return true;
}

QPointF Calibration::getPointOnScreen(QPointF point) {
  qreal x;
  qreal y;

  x = ax*point.x() + bx*point.y() + cx;
  y = ay*point.x() + by*point.y() + cy;

  return QPointF(x, y);
}
