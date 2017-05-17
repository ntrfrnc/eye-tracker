#include "sessionhandler.h"
#include <QDebug>
#include <QGridLayout>
#include <QKeyEvent>
#include <QPainter>
#include <QPointF>
#include <QSizePolicy>
#include <QWebEngineView>

SessionHandler::SessionHandler() {}

void SessionHandler::setFilePath(QString path){
  this->filePath = path;
};

void SessionHandler::setBgUrl(QUrl url){
  this->bgUrl = url;
};

void SessionHandler::setSerialPort(QString serialPortName){
  this->serialPortName = serialPortName;
};

void SessionHandler::setCalibration(Calibration *calibration){
  this->calibration = calibration;
};

void SessionHandler::start() {
  // Create and show session board
  setWindowState(Qt::WindowFullScreen);

  QWebEngineView *bg = new QWebEngineView;
  bg->load(bgUrl);

  QGridLayout *l = new QGridLayout;
  l->setMargin(0);
  l->addWidget(bg, 0, 0, 1, 1);
  l->addWidget(&pointerWidget, 0, 0, 1, 1);

  setLayout(l);
  grabKeyboard();
  show();

  pointerWidget.setPoint(QPointF(960.0, 540.0));
  pointerWidget.setCalibration(calibration);

  connect(&positionReader, &DataHandler::eyePositionRead, &pointerWidget, &EyePointerWidget::setPoint);
  positionReader.startReading(serialPortName);

  timer.restart();

  outputFile.setFileName(filePath);
  outputFile.open(QFile::ReadWrite | QFile::Text);
  outputFileStream.setDevice(&outputFile);
  if (outputFile.pos() == 0) {
    // Add header if file is empty
    outputFileStream << "Time [ms], Eye position X [px], Eye position Y [px] \n";
  }

  pointerWidget.show();
}

void SessionHandler::stop() {
  releaseKeyboard();
  disconnect(&positionReader, &DataHandler::eyePositionRead, &pointerWidget, &EyePointerWidget::setPoint);
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

void SessionHandler::writePointToFile(QPointF point){
  QPointF sPoint = calibration->getPointOnScreen(point);
  outputFileStream << timer.elapsed() << "," << sPoint.x() << "," << sPoint.y();
};
