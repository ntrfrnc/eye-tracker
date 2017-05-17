#ifndef CALIBRATIONHANDLER_H
#define CALIBRATIONHANDLER_H

#include <QWebEngineView>
#include "calibration.h"
#include "datahandler.h"

class CalibrationHandler : public QWebEngineView {
  QPointF *ePs;  // eyetracker calibration points
  uint spaceCounter;
  QPointF currentPoint;
  QString serialPortName;
  DataHandler positionReader;

  void keyPressEvent(QKeyEvent *event);

 public:
  Calibration calibration;

  CalibrationHandler();
  void start();
  void stop();

 public slots:
  void setCurrentPoint(QPointF point);
  void setSerialPort(QString serialPortName);
};

#endif  // CALIBRATIONHANDLER_H
