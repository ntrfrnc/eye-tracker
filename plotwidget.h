#ifndef PLOTWIDGET_H
#define PLOTWIDGET_H

#include <QWidget>

class PlotWidget : public QWidget {
  Q_OBJECT

  QVector<QPointF> points;
  void paintEvent(QPaintEvent *);

 public:
  explicit PlotWidget(QWidget *parent = 0);
  void clearPoints();
 signals:

 public slots:
  void addPoint(QPointF point);
};

#endif  // PLOTWIDGET_H
