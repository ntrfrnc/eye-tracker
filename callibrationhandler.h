#ifndef CALLIBRATIONHANDLER_H
#define CALLIBRATIONHANDLER_H

#include <QWebEngineView>
#include "calibration.h"
#include "datahandler.h"

class CallibrationHandler : public QWebEngineView {
  QPointF *ePs;  // eyetracker calibration points
  uint spaceCounter;
  QPointF currentPoint;
  QString serialPortName;
  DataHandler positionReader;

  void keyPressEvent(QKeyEvent *event);

 public:
  Calibration calibration;

  CallibrationHandler();
  void start();
  void stop();

 public slots:
  void setCurrentPoint(QPointF point);
  void setSerialPort(QString serialPortName);
};

#endif  // CALLIBRATIONHANDLER_H
