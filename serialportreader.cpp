#include "serialportreader.h"
#include <QDebug>

SerialPortReader::SerialPortReader(QSerialPort *serialPort,
                                   QByteArray frameStartMarker,
                                   QByteArray frameEndMarker,
                                   qint16 frameLength, QObject *parent)
    : QObject(parent),
      frameStartMarker(frameStartMarker),
      frameEndMarker(frameEndMarker),
      frameLength(frameLength),
      serialPort(serialPort) {}

SerialPortReader::SerialPortReader() {}

SerialPortReader::~SerialPortReader() {}

void SerialPortReader::setSerialPort(QSerialPort *serialPort) {
  this->serialPort = serialPort;
};

void SerialPortReader::setFrameLength(qint16 frameLength) {
  this->frameLength = frameLength;
}

void SerialPortReader::setFrameStartMarker(QByteArray frameStartMarker) {
  this->frameStartMarker = frameStartMarker;
};

void SerialPortReader::setFrameEndMarker(QByteArray frameEndMarker) {
  this->frameEndMarker = frameEndMarker;
};

void SerialPortReader::start() {
  connect(serialPort, &QSerialPort::readyRead, this,
          &SerialPortReader::handleReadyRead);

  connect(serialPort,
          static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(
              &QSerialPort::error),
          this, &SerialPortReader::handleError);
};

void SerialPortReader::handleReadyRead() {
  if (serialPort->bytesAvailable() < frameLength) {
    return;
  }

  QByteArray data = serialPort->readAll();
  // qDebug() << data;
  //  qDebug() << m_frameLength;
  //  qDebug() << m_frameMarker;
  dataBuffer.append(data);
  // qDebug() << "data too short";
  if (dataBuffer.size() < frameLength) {
    return;  // data to short
  }
  // qDebug() << "frame marker not found";
  int indexOfFrame = dataBuffer.indexOf(frameEndMarker + frameStartMarker) +
                     frameEndMarker.length();
  // qDebug() << m_frameMarker;
  if (indexOfFrame < 0) {
    return;  // frame marker not found
  }
  // qDebug() << "frame is not complete";
  if ((dataBuffer.size() - indexOfFrame) < frameLength) {
    return;  // frame is not complete
  }
  //  qDebug() << dataBuffer;
  // qDebug() << "jest ok";
  // qDebug() << indexOfFrame;
  QByteArray frame = dataBuffer.mid(indexOfFrame, frameLength);
  // qDebug() << frame;
  // Remove processed data from the buffer
  dataBuffer = dataBuffer.right(indexOfFrame + frameLength);

  // Invoke frameRead signal
  frameRead(frame);
}

void SerialPortReader::handleError(
    QSerialPort::SerialPortError serialPortError) {
  if (serialPortError == QSerialPort::ReadError) {
    errorHandler.showMessage(
        tr("An I/O error occurred while reading the data from "
           "port %1, error: %2")
            .arg(serialPort->portName())
            .arg(serialPort->errorString()));
  }
}
