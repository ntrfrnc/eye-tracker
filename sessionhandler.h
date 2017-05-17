#ifndef SESSIONHANDLER_H
#define SESSIONHANDLER_H

#include <eyepointerwidget.h>
#include <QFile>
#include <QTextStream>
#include <QUrl>
#include <QWidget>
#include <QTime>
#include "calibration.h"
#include "datahandler.h"

class SessionHandler : public QWidget {
  QString filePath;
  QUrl bgUrl;
  QString serialPortName;
  EyePointerWidget pointerWidget;
  DataHandler positionReader;
  Calibration *calibration;
  QFile outputFile;
  QTextStream outputFileStream;
  QTime timer;

  void keyPressEvent(QKeyEvent *event);

 public:
  SessionHandler();
  void setFilePath(QString path);
  void setBgUrl(QUrl url);
  void setSerialPort(QString serialPortName);
  void setCalibration(Calibration *calibration);
  void start();
  void stop();

 public slots:
  void writePointToFile(QPointF point);
};

#endif  // SESSIONHANDLER_H
