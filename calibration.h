#ifndef CALIBRATION_H
#define CALIBRATION_H

#include <QObject>

class Calibration : public QObject
{
    Q_OBJECT
public:
    explicit Calibration(QObject *parent = 0);

signals:

public slots:
};

#endif // CALIBRATION_H