#ifndef CALLIBRATIONHANDLER_H
#define CALLIBRATIONHANDLER_H

#include <QWebEngineView>

class CallibrationHandler : public QWebEngineView {
  void keyPressEvent(QKeyEvent *event);

 public:
  int screenWidth;
  int screenHeight;

  CallibrationHandler();
  void start();
  void stop();
};

#endif  // CALLIBRATIONHANDLER_H
