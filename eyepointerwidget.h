#ifndef EYEPOINTERWIDGET_H
#define EYEPOINTERWIDGET_H

#include <QPointF>
#include <QWidget>

class EyePointerWidget : public QWidget {
  Q_OBJECT

  QPointF point;
  void paintEvent(QPaintEvent *);

 public:
  explicit EyePointerWidget(QWidget *parent = 0);

 signals:

 public slots:
  void setPoint(QPointF point);
};

#endif  // EYEPOINTERWIDGET_H
