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
  uint sessionCounter;

 public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

 private slots:
  void on_startCallibrationPushButton_clicked();
  void on_startNewSessionPushButton_clicked();
  void on_stopSessionPushButton_clicked();
  void on_browseFileNamePushButton_clicked();
  void on_browseBoardBgPushButton_clicked();
  void on_serialPortComboBox_currentIndexChanged(const QString &arg1);

 private:
  Ui::MainWindow *ui;
};

#endif  // MAINWINDOW_H
