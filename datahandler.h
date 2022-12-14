#ifndef DATAHANDLER_H
#define DATAHANDLER_H

#include "serialportreader.h"

#include <QObject>
#include <QString>
#include <QtSerialPort>

struct Packet {
  // frame 1
  QString synchronization;
  QString eye_y_0;
  QString eye_x_0;
  QString acc_x_0;
  QString acc_y_0;
  QString gyro_x_0;
  QString gyro_y_0;
  QString mic_0;
  QString mic_1;
  QString mic_2;
  QString mic_3;
  QString mic_4;
  QString mic_5;
  QString mic_6;
  QString mic_7;
  QString C1;
  QString eye_b;
  QString COUNTER;

  // frame 2
  QString pul_I;
  QString pul_R;
  QString eye_y_1;
  QString eye_x_1;
  QString acc_x_1;
  QString acc_y_1;
  QString gyro_x_1;
  QString gyro_y_1;
  QString mic_8;
  QString mic_9;
  QString mic_10;
  QString mic_11;
  QString mic_12;
  QString mic_13;
  QString mic_14;
  QString mic_15;
  QString C2;
  QString CRC;
  QString C3;
};

class DataHandler : public QObject {
  Q_OBJECT

  qint32 baudRate;
  QSerialPort::DataBits dataBits;
  QSerialPort::StopBits stopBits;
  QSerialPort::Parity parity;
  QByteArray frameStartMarker;
  QByteArray frameEndMarker;
  qint16 frameLength;

  QSerialPort serialPort;
  SerialPortReader dataReader;

  QString lastErrorMsg;

  QVector<qint32> xBuffer;
  QVector<qint32> yBuffer;
  qint32 xOffset;
  qint32 yOffset;

  void trackOffsetX();
  void trackOffsetY();
  qint32 calcOffsetX();
  qint32 calcOffsetY();
  qint32 bit12ToInt(QString input);
  void readEyePosition(qint32 eyeX, qint32 eyeY);

 public:
  explicit DataHandler(QObject *parent = 0);
  Packet getPacket(QByteArray frame);
  bool startReading(QString serialPortName);
  void stopReading();
  QString errorString();

 signals:
  void eyePositionRead(QPointF point);

 public slots:
  void readHandler(QByteArray frame);
};

#endif  // DATAHANDLER_H
