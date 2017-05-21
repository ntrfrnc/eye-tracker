#include "sessionhandler.h"
#include <QDebug>
#include <QKeyEvent>
#include <QPainter>
#include <QPointF>
#include <QSizePolicy>

SessionHandler::SessionHandler() {
  layout.setMargin(0);
  layout.addWidget(&background, 0, 0, 1, 1);
  layout.addWidget(&pointerWidget, 0, 0, 1, 1);
  setLayout(&layout);
}

void SessionHandler::setFilePath(QString path) { this->filePath = path; };

void SessionHandler::setBgUrl(QUrl url) { this->bgUrl = url; };

void SessionHandler::setSerialPort(QString serialPortName) {
  this->serialPortName = serialPortName;
};

void SessionHandler::setCalibration(Calibration *calibration) {
  this->calibration = calibration;
};

void SessionHandler::start() {
  if (!positionReader.startReading(serialPortName)) {
    errorHandler.showMessage("Can't connect to serial port.");
    positionReader.stopReading();
    return;
  };

  if(!calibration->isCalculated()){
    errorHandler.showMessage("Calibration was not performed. You need to calibrate device first.");
    positionReader.stopReading();
    return;
  }

  pointerWidget.setCalibration(calibration);

  // Create and show session board
  setWindowState(Qt::WindowFullScreen);
  background.load(bgUrl);
  grabKeyboard();
  show();

  pointerWidget.setPoint(QPointF(960, 520));

  connect(&positionReader, &DataHandler::eyePositionRead, &pointerWidget,
          &EyePointerWidget::setEyesPosition);

  timer.restart();

  outputFile.setFileName(filePath);
  if (outputFile.open(QFile::ReadWrite | QFile::Text)) {
    outputFileStream.setDevice(&outputFile);
    if (outputFile.pos() == 0) {
      // Add header if file is empty
      outputFileStream
          << "Time [ms], Eye position X [px], Eye position Y [px] \n";
    }
    connect(&positionReader, &DataHandler::eyePositionRead, this,
            &SessionHandler::writePointToFile);
  } else {
    errorHandler.showMessage("Can't open/create file to record data.");
  }

  pointerWidget.setAttribute(Qt::WA_TransparentForMouseEvents);
  pointerWidget.show();
}

void SessionHandler::stop() {
  releaseKeyboard();
  disconnect(&positionReader, &DataHandler::eyePositionRead, this,
             &SessionHandler::writePointToFile);
  disconnect(&positionReader, &DataHandler::eyePositionRead, &pointerWidget,
             &EyePointerWidget::setPoint);
  positionReader.stopReading();
  outputFile.close();
  hide();
}

void SessionHandler::keyPressEvent(QKeyEvent *ke) {
  if (ke->isAutoRepeat()) {
    return;
  }

  switch (ke->key()) {
    case Qt::Key_Escape:
      stop();
      break;
  }
}

void SessionHandler::writePointToFile(QPointF point) {
  QPointF sPoint = calibration->getPointOnScreen(point);
  outputFileStream << timer.elapsed() << "," << sPoint.x() << "," << sPoint.y()
                   << "\n";
};
