#include "calibrationhandler.h"
#include <QGuiApplication>
#include <QKeyEvent>
#include <QRect>
#include <QScreen>

CalibrationHandler::CalibrationHandler() {}

void CalibrationHandler::setSerialPort(QString serialPortName) {
  this->serialPortName = serialPortName;
}

void CalibrationHandler::start() {
  if (!positionReader.startReading(serialPortName)) {
    errorHandler.showMessage(tr("Can't connect to serial port. Error: %1")
                                 .arg(positionReader.errorString()));
    return;
  }

  connect(&positionReader, &DataHandler::eyePositionRead, this,
          &CalibrationHandler::setCurrentPoint);

  // Get screen resolution
  QScreen *screen = QGuiApplication::primaryScreen();
  QRect screenGeometry = screen->geometry();

  calibration.setScreenSize(screenGeometry.width(), screenGeometry.height());
  ePs = new QPointF[5];
  spaceCounter = 1;

  // Create and show callibration board
  load(QUrl("qrc:/callibrationBoard.html"));
  grabKeyboard();
  showFullScreen();
}

void CalibrationHandler::stop() {
  spaceCounter = 1;
  releaseKeyboard();
  disconnect(&positionReader, &DataHandler::eyePositionRead, this,
             &CalibrationHandler::setCurrentPoint);
  positionReader.stopReading();
  hide();
}

void CalibrationHandler::setCurrentPoint(QPointF point) {
  currentPoint = point;
}

void CalibrationHandler::keyPressEvent(QKeyEvent *ke) {
  if (ke->isAutoRepeat()) {
    return;
  }

  switch (ke->key()) {
    case Qt::Key_Space:
      switch (spaceCounter) {
        case 1:
          ePs[0] = currentPoint;  // Top left
          break;

        case 2:
          ePs[1] = currentPoint;  // Center
          break;

        case 3:
          ePs[2] = currentPoint;  // Bottom right
          break;

        case 4:
          ePs[3] = currentPoint;  // Bottom left
          break;

        case 5:
          // Center 2nd time
          ePs[1].setX((ePs[1].x() + currentPoint.x()) / 2);
          ePs[1].setY((ePs[1].y() + currentPoint.y()) / 2);
          break;

        case 6:
          ePs[4] = currentPoint;  // Top right
          stop();

          if(!calibration.calculateFactors(ePs)){
            errorHandler.showMessage("Error occured during calibration. Please try again.");
          }
          break;
      }

      spaceCounter++;
      break;

    case Qt::Key_Escape:
      stop();
      break;
  }
}
