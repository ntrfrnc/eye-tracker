#ifndef SESSIONHANDLER_H
#define SESSIONHANDLER_H

#include <eyepointerwidget.h>
#include <QUrl>
#include <QWidget>
#include "datahandler.h"

class SessionHandler : public QWidget {
  QUrl filePath;
  QUrl bgUrl;
  QString serialPortName;
  EyePointerWidget pointerWidget;
  DataHandler positionReader;

  void keyPressEvent(QKeyEvent *event);

 public:
  SessionHandler(QUrl filePath, QUrl bgUrl, QString serialPort);
  void start();
  void stop();
};

#endif  // SESSIONHANDLER_H
