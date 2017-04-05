#include "mainwindow.h"
#include "callibrationhandler.h"
#include "sessionhandler.h"
#include "ui_mainwindow.h"

#include <QtSerialPort/QSerialPortInfo>
#include <QUrl>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  const auto infos = QSerialPortInfo::availablePorts();
  for (const QSerialPortInfo &info : infos) {
    ui->serialPortComboBox->addItem(info.portName());
  }

  ui->setupUi(this);

  ui->fileNameLineEdit->setText(tr("output.txt"));
  ui->boardBgLineEdit->setText(tr("http://google.com"));
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_startCallibrationPushButton_clicked() {
  CallibrationHandler *callibration = new CallibrationHandler;
  callibration->start();
}

void MainWindow::on_startNewSessionPushButton_clicked() {
  SessionHandler *session = new SessionHandler(QUrl(ui->fileNameLineEdit->text()), QUrl(ui->boardBgLineEdit->text()), ui->serialPortComboBox->currentText());
  session->start();
}
