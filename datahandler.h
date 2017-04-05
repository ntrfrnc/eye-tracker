#ifndef DATAHANDLER_H
#define DATAHANDLER_H

#include <QObject>
#include <QString>

// struktura przechowywująca sygnały z urządzenia
struct Packet {
  // ramka 1
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

  // ramka 2
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

 public:
  explicit DataHandler(QObject *parent = 0);

 signals:

 public slots:
};

#endif  // DATAHANDLER_H
