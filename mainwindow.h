#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "calibrationhandler.h"
#include "sessionhandler.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

  SessionHandler sessionHandler;
  CalibrationHandler calibrationHandler;

 public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

 private slots:
  void on_startCallibrationPushButton_clicked();
  void on_startNewSessionPushButton_clicked();

 private:
  Ui::MainWindow *ui;
};

#endif  // MAINWINDOW_H
