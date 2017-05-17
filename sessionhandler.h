#ifndef SESSIONHANDLER_H
#define SESSIONHANDLER_H

#include <eyepointerwidget.h>
#include <QUrl>
#include <QWidget>
#include "datahandler.h"
#include "calibration.h"

class SessionHandler : public QWidget {
  QUrl filePath;
  QUrl bgUrl;
  QString serialPortName;
  EyePointerWidget pointerWidget;
  DataHandler positionReader;
  Calibration *calibration;

  void keyPressEvent(QKeyEvent *event);

 public:
  SessionHandler();
  void setFilePath(QUrl path);
  void setBgUrl(QUrl url);
  void setSerialPort(QString serialPortName);
  void setCalibration(Calibration *calibration);
  void start();
  void stop();
};

#endif  // SESSIONHANDLER_H
