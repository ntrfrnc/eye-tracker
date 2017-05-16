#include "serialportreader.h"
#include <QDebug>

SerialPortReader::SerialPortReader(QSerialPort *serialPort, QByteArray frameMarker,
                                   qint16 frameLength, QObject *parent)
    : QObject(parent),
      m_frameMarker(frameMarker),
      m_frameLength(frameLength),
      m_serialPort(serialPort),
      m_standardOutput(stdout) {}

SerialPortReader::SerialPortReader() {}

SerialPortReader::~SerialPortReader() {}

void SerialPortReader::setSerialPort(QSerialPort *serialPort) {
  m_serialPort = serialPort;
};

void SerialPortReader::setFrameLength(qint16 frameLength) {
  m_frameLength = frameLength;
}

void SerialPortReader::setFrameMarker(QByteArray frameMarker) {
  m_frameMarker = frameMarker;
};

void SerialPortReader::start() {
  connect(m_serialPort, &QSerialPort::readyRead, this,
          &SerialPortReader::handleReadyRead);

  connect(m_serialPort,
          static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(
              &QSerialPort::error),
          this, &SerialPortReader::handleError);
};

void SerialPortReader::handleReadyRead() {
//    if (m_serialPort->bytesAvailable() < m_frameLength) {
//      return;
//    }

  QByteArray data = m_serialPort->readAll();
// qDebug() << data;
//  qDebug() << m_frameLength;
//  qDebug() << m_frameMarker;
  dataBuffer.append(data);
//qDebug() << "data too short";
  if (dataBuffer.size() < m_frameLength) {
    return;  // data to short
  }
//qDebug() << "frame marker not found";
  int indexOfFrame = dataBuffer.indexOf(m_frameMarker);
//qDebug() << m_frameMarker;
  if (indexOfFrame < 0) {
    return;  // frame marker not found
  }
//qDebug() << "frame is not complete";
  if ((dataBuffer.size() - indexOfFrame) < m_frameLength) {
    return;  // frame is not complete
  }
//  qDebug() << dataBuffer;
//qDebug() << "jest ok";
//qDebug() << indexOfFrame;
  QByteArray frame = dataBuffer.mid(indexOfFrame, m_frameLength);
//qDebug() << frame;
  // Remove processed data from the buffer
  dataBuffer = dataBuffer.right(indexOfFrame + m_frameLength);

  // Invoke frameRead signal
  frameRead(frame);
}

void SerialPortReader::handleError(
    QSerialPort::SerialPortError serialPortError) {
  if (serialPortError == QSerialPort::ReadError) {
    m_standardOutput << QObject::tr(
                            "An I/O error occurred while reading the data from "
                            "port %1, error: %2")
                            .arg(m_serialPort->portName())
                            .arg(m_serialPort->errorString())
                     << endl;
  }
}
