#ifndef SESSIONHANDLER_H
#define SESSIONHANDLER_H

#include <eyepointerwidget.h>
#include <QErrorMessage>
#include <QFile>
#include <QGridLayout>
#include <QTextStream>
#include <QTime>
#include <QUrl>
#include <QWebEngineView>
#include <QWidget>

#include "calibration.h"
#include "datahandler.h"

class SessionHandler : public QWidget {
  bool pointerStatus;
  QString filePath;
  QUrl bgUrl;
  QString serialPortName;
  QWebEngineView background;
  QGridLayout layout;
  EyePointerWidget pointerWidget;
  DataHandler positionReader;
  Calibration *calibration;
  QFile outputFile;
  QTextStream outputFileStream;
  QTime timer;
  QErrorMessage errorHandler;

  void keyPressEvent(QKeyEvent *event);

 public:
  uint sessionCounter;

  SessionHandler();
  void setFilePath(QString path);
  void setBgUrl(QUrl url);
  void setSerialPort(QString serialPortName);
  void setPointerStatus(bool status);
  void setCalibration(Calibration *calibration);
  bool start();
  void stop();

 public slots:
  void writePointToFile(QPointF point);
};

#endif  // SESSIONHANDLER_H
