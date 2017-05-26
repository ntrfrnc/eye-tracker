#include "datahandler.h"

#include <string>

DataHandler::DataHandler(QObject *parent) : QObject(parent) {
  baudRate = 300000;
  dataBits = QSerialPort::Data8;
  stopBits = QSerialPort::OneStop;
  parity = QSerialPort::NoParity;
  frameMarker = QByteArray::fromHex("000000");
  frameLength = 56;

  dataReader.setSerialPort(&serialPort);
  dataReader.setFrameLength(frameLength);
  dataReader.setFrameMarker(frameMarker);
  dataReader.start();
}

QString DataHandler::errorString(){
    return lastErrorMsg;
}

bool DataHandler::startReading(QString serialPortName) {
  serialPort.setPortName(serialPortName);

  serialPort.setBaudRate(baudRate);
  serialPort.setStopBits(stopBits);
  serialPort.setParity(parity);
  serialPort.setDataBits(dataBits);

  if (!serialPort.open(QIODevice::ReadOnly)) {
      lastErrorMsg = serialPort.errorString();
      return false;
  };

  connect(&dataReader, &SerialPortReader::frameRead, this,
          &DataHandler::readHandler);

  return true;
}

void DataHandler::stopReading() {
  serialPort.close();
  disconnect(&dataReader, &SerialPortReader::frameRead, this,
             &DataHandler::readHandler);
}

void DataHandler::readHandler(QByteArray frame) {
  Packet packet = getPacket(frame);
//  qDebug() << "synchro: " << packet.synchronization;
//  qDebug() << "x: " << packet.eye_x_0;
//  qDebug() << "y: " << packet.eye_y_0;
  int x = bit12ToInt(packet.eye_x_0);
  int y = bit12ToInt(packet.eye_y_0);
  QPointF point(x, y);
  qDebug() << "x: " << x;
  qDebug() << "y: " << y;

  // Invoke eyePositionRead signal
  eyePositionRead(point);
}

int DataHandler::bit12ToInt(QString input) {
  int x = input.toInt(Q_NULLPTR, 2);
  // x = (x >> 11) == 0 ? x : (-1 ^ 0xFFF) | x;

  return x;
}

Packet DataHandler::getPacket(QByteArray frame) {
  Packet packet;
  int counter = 0;
  char bit;

  // zamiana bajtów na bity i przypisanie ich do sygnałów
  for (int i = 0; i < frame.size(); ++i) {
    for (int j = 0; j < 8; j++) {
      bit = 48 + (frame[i] >> (7 - j) & 1);
      if (counter < 24) packet.synchronization += bit;
      if (counter > 23 && counter < 36) packet.eye_y_0 += bit;
      if (counter > 35 && counter < 48) packet.eye_x_0 += bit;
      if (counter > 47 && counter < 60) packet.acc_x_0 += bit;
      if (counter > 59 && counter < 72) packet.acc_y_0 += bit;
      if (counter > 71 && counter < 84) packet.gyro_x_0 += bit;
      if (counter > 83 && counter < 96) packet.gyro_y_0 += bit;
      if (counter > 95 && counter < 108) packet.mic_0 += bit;
      if (counter > 107 && counter < 120) packet.mic_1 += bit;
      if (counter > 119 && counter < 132) packet.mic_2 += bit;
      if (counter > 131 && counter < 144) packet.mic_3 += bit;
      if (counter > 143 && counter < 156) packet.mic_4 += bit;
      if (counter > 155 && counter < 168) packet.mic_5 += bit;
      if (counter > 167 && counter < 180) packet.mic_6 += bit;
      if (counter > 179 && counter < 192) packet.mic_7 += bit;
      if (counter > 191 && counter < 196) packet.C1 += bit;
      if (counter > 195 && counter < 208) packet.eye_b += bit;
      if (counter > 207 && counter < 224) packet.COUNTER += bit;
      if (counter > 223 && counter < 236) packet.pul_I += bit;
      if (counter > 235 && counter < 248) packet.pul_R += bit;
      if (counter > 247 && counter < 260) packet.eye_y_1 += bit;
      if (counter > 259 && counter < 272) packet.eye_x_1 += bit;
      if (counter > 271 && counter < 284) packet.acc_x_1 += bit;
      if (counter > 283 && counter < 296) packet.acc_y_1 += bit;
      if (counter > 295 && counter < 308) packet.gyro_x_1 += bit;
      if (counter > 307 && counter < 320) packet.gyro_y_1 += bit;
      if (counter > 319 && counter < 332) packet.mic_8 += bit;
      if (counter > 331 && counter < 344) packet.mic_9 += bit;
      if (counter > 343 && counter < 356) packet.mic_10 += bit;
      if (counter > 355 && counter < 368) packet.mic_11 += bit;
      if (counter > 367 && counter < 380) packet.mic_12 += bit;
      if (counter > 379 && counter < 392) packet.mic_13 += bit;
      if (counter > 391 && counter < 404) packet.mic_14 += bit;
      if (counter > 403 && counter < 416) packet.mic_15 += bit;
      if (counter > 415 && counter < 424) packet.C2 += bit;
      if (counter > 423 && counter < 440) packet.CRC += bit;
      if (counter > 439) packet.C3 += bit;
      counter++;
    }
  }

  return packet;
}
