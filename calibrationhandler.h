#ifndef CALIBRATIONHANDLER_H
#define CALIBRATIONHANDLER_H

#include <QErrorMessage>
#include <QWebEngineView>
#include "calibration.h"
#include "datahandler.h"

class CalibrationHandler : public QWebEngineView {
  Q_OBJECT

  QPointF *ePs;  // eyetracker calibration points
  uint spaceCounter;
  QVector<QPointF> signalBuffer;
  QString serialPortName;
  DataHandler positionReader;
  QErrorMessage errorHandler;

  void keyPressEvent(QKeyEvent *event);

 public:
  Calibration calibration;

  CalibrationHandler();
  QPointF getAveragePosition();
  void start();
  void stop();

 signals:
  void stopped();

 public slots:
  void setCurrentPoint(QPointF point);
  void setSerialPort(QString serialPortName);
};

#endif  // CALIBRATIONHANDLER_H
