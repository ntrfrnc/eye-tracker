#ifndef SERIALPORTREADER_H
#define SERIALPORTREADER_H

#include <QtSerialPort/QSerialPort>

#include <QByteArray>
#include <QObject>
#include <QTextStream>

class SerialPortReader : public QObject {
  Q_OBJECT

 public:
  explicit SerialPortReader(QSerialPort *serialPort, QString frameMarker,
                            qint16 frameLength, QObject *parent = nullptr);
  explicit SerialPortReader();
  ~SerialPortReader();

  void setSerialPort(QSerialPort *serialPort);
  void setFrameLength(qint16 frameLength);
  void setFrameMarker(QString frameMarker);
  void start();

 signals:
  void frameRead(QByteArray frame);

 private slots:
  void handleReadyRead();
  void handleError(QSerialPort::SerialPortError error);

 private:
  QString m_frameMarker;
  qint16 m_frameLength;
  QSerialPort *m_serialPort;
  QByteArray dataBuffer;
  QTextStream m_standardOutput;
};

#endif  // SERIALPORTREADER_H
