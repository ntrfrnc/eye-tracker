#include "sessionhandler.h"
#include <QKeyEvent>
#include <QMessageBox>
#include <QPointF>
#include <QSizePolicy>

SessionHandler::SessionHandler() : pointerStatus(true), sessionCounter(1) {
  layout.setMargin(0);
  layout.addWidget(&background, 0, 0, 1, 1);
  layout.addWidget(&pointerWidget, 0, 0, 1, 1);
  setLayout(&layout);
}

void SessionHandler::setPointerStatus(bool status) { pointerStatus = status; };

void SessionHandler::setFilePath(QString path) { this->filePath = path; };

void SessionHandler::setBgUrl(QUrl url) { this->bgUrl = url; };

void SessionHandler::setSerialPort(QString serialPortName) {
  this->serialPortName = serialPortName;
};

void SessionHandler::setCalibration(Calibration *calibration) {
  this->calibration = calibration;
};

bool SessionHandler::start() {
  if (!positionReader.startReading(serialPortName)) {
    errorHandler.showMessage(tr("Can't connect to serial port. Error: %1")
                                 .arg(positionReader.errorString()));
    positionReader.stopReading();
    return false;
  };

  if (!calibration->isCalculated()) {
    errorHandler.showMessage(
        "Calibration was not performed. You need to calibrate device first.");
    positionReader.stopReading();
    return false;
  }

  outputFile.setFileName(filePath);

  if (outputFile.exists()) {
    QMessageBox msgBox;
    msgBox.setWindowTitle("File already exists");
    msgBox.setText(tr("File '%1' already exists. Do you want to override it?")
                       .arg(filePath));
    msgBox.setStandardButtons(QMessageBox::Yes);
    msgBox.addButton(QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);

    if (msgBox.exec() == QMessageBox::No) {
      positionReader.stopReading();
      return false;
    }
  }

  if (!outputFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
    errorHandler.showMessage("Can't open/create file to record data.");
    positionReader.stopReading();
    return false;
  }

  // Create and show session board
  background.load(bgUrl);
  background.show();
  grabKeyboard();
  showFullScreen();

  if (pointerStatus) {
    pointerWidget.setCalibration(calibration);
    pointerWidget.setPoint(QPointF(960, 520));

    connect(&positionReader, &DataHandler::eyePositionRead, &pointerWidget,
            &EyePointerWidget::setEyesPosition);

    pointerWidget.setAttribute(Qt::WA_TransparentForMouseEvents);
    pointerWidget.show();
  } else {
    pointerWidget.hide();
  }

  timer.restart();
  sessionCounter++;

  outputFileStream.setDevice(&outputFile);

  if (outputFile.pos() == 0) {
    // Add header if file is empty
    outputFileStream
        << "sep=,\n"
        << tr("Session:, %1\n").arg(sessionCounter)
        << tr("Date:, %1\n").arg(QDateTime::currentDateTime().toString(Qt::ISODate))
        << tr("Screen width [px]:, %1\n").arg(calibration->getScreenWidth())
        << tr("Screen height [px]:, %1\n").arg(calibration->getScreenHeight())
        << "Time [ms], Eye position X [px], Eye position Y [px] \n";
  }

  connect(&positionReader, &DataHandler::eyePositionRead, this,
          &SessionHandler::writePointToFile);

  return true;
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

  emit stopped();
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
