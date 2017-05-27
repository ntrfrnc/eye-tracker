#ifndef SERIALPORTREADER_H
#define SERIALPORTREADER_H

#include <QByteArray>
#include <QErrorMessage>
#include <QObject>
#include <QTextStream>
#include <QtSerialPort/QSerialPort>

class SerialPortReader : public QObject {
  Q_OBJECT

  QErrorMessage errorHandler;
  QByteArray frameStartMarker;
  QByteArray frameEndMarker;
  qint16 frameLength;
  QSerialPort *serialPort;
  QByteArray dataBuffer;

 public:
  explicit SerialPortReader(QSerialPort *serialPort,
                            QByteArray frameStartMarker,
                            QByteArray frameEndMarker, qint16 frameLength,
                            QObject *parent = nullptr);
  explicit SerialPortReader();
  ~SerialPortReader();

  void setSerialPort(QSerialPort *serialPort);
  void setFrameLength(qint16 frameLength);
  void setFrameStartMarker(QByteArray frameStartMarker);
  void setFrameEndMarker(QByteArray frameEndMarker);
  void start();

 signals:
  void frameRead(QByteArray frame);

 public slots:
  void handleReadyRead();
  void handleError(QSerialPort::SerialPortError error);
};

#endif  // SERIALPORTREADER_H
