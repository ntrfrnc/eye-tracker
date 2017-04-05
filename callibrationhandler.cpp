#include "callibrationhandler.h"
#include <QDebug>
#include <QGuiApplication>
#include <QKeyEvent>
#include <QRect>
#include <QScreen>

CallibrationHandler::CallibrationHandler() {}

void CallibrationHandler::start() {
  // Create and show callibration board
  load(QUrl("qrc:///callibrationBoard.html"));
  setWindowState(Qt::WindowFullScreen);
  grabKeyboard();
  show();

  // Get screen resolution
  QScreen *screen = QGuiApplication::primaryScreen();
  QRect screenGeometry = screen->geometry();
  screenHeight = screenGeometry.height();
  screenWidth = screenGeometry.width();
}

void CallibrationHandler::stop() {
    releaseKeyboard();
    hide();
}

void CallibrationHandler::keyPressEvent(QKeyEvent *ke) {
  if (ke->isAutoRepeat()) {
    return;
  }

  switch (ke->key()) {
    case Qt::Key_Space:
      qDebug() << "spacepressed";
      break;

    case Qt::Key_Escape:
      stop();
      break;
  }
}
