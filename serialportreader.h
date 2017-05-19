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
  QByteArray m_frameMarker;
  qint16 m_frameLength;
  QSerialPort *m_serialPort;
  QByteArray dataBuffer;
  QTextStream m_standardOutput;

 public:
  explicit SerialPortReader(QSerialPort *serialPort, QByteArray frameMarker,
                            qint16 frameLength, QObject *parent = nullptr);
  explicit SerialPortReader();
  ~SerialPortReader();

  void setSerialPort(QSerialPort *serialPort);
  void setFrameLength(qint16 frameLength);
  void setFrameMarker(QByteArray frameMarker);
  void start();

 signals:
  void frameRead(QByteArray frame);

 public slots:
  void handleReadyRead();
  void handleError(QSerialPort::SerialPortError error);
};

#endif  // SERIALPORTREADER_H
