#ifndef DATAPLOTTER_H
#define DATAPLOTTER_H

#include "plotwidget.h"

#include <QErrorMessage>
#include <QFile>
#include <QGridLayout>
#include <QObject>
#include <QTextStream>
#include <QWebEngineView>
#include <QWidget>

class DataPlotter : public QWidget {
  Q_OBJECT;

  QString outImageFilePath;
  QUrl bgUrl;
  QWebEngineView background;
  PlotWidget plotter;
  QGridLayout layout;
  QErrorMessage errorHandler;

  void keyPressEvent(QKeyEvent *event);

 public:
  explicit DataPlotter();
  void setOutImageFilePath(QString path);
  void setBgUrl(QUrl url);
  void plot(QString dataFilePath);
  void start(QString dataFilePath);
  void stop();

 signals:
  void stopped();

 public slots:
  void saveOutputImage();
};

#endif  // DATAPLOTTER_H
