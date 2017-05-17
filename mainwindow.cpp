#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTextBrowser>
#include <QUrl>
#include <QtSerialPort/QSerialPortInfo>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  //    ui->serialPortComboBox->addItem(QString("COM3"));
  //    ui->serialPortComboBox->addItem(QString("/dev/ttyS0"));
  //    ui->serialPortComboBox->addItem(QString("/dev/ttyUSB0"));
  //    ui->serialPortComboBox->addItem(QString("/dev/pts/3"));

  //  const auto infos = QSerialPortInfo::availablePorts();
  //  for (const QSerialPortInfo &info : infos) {
  //    ui->serialPortComboBox->addItem(info.portName());
  //  }

  ui->setupUi(this);

  ui->fileNameLineEdit->setText(tr("output.csv"));
  ui->boardBgLineEdit->setText(tr("http://google.com"));
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_startCallibrationPushButton_clicked() {
  calibrationHandler.setSerialPort(ui->serialPortNameLineEdit->text());
  calibrationHandler.start();
}

void MainWindow::on_startNewSessionPushButton_clicked() {
  sessionHandler.setFilePath(ui->fileNameLineEdit->text());
  sessionHandler.setBgUrl(QUrl(ui->boardBgLineEdit->text()));
  sessionHandler.setSerialPort(ui->serialPortNameLineEdit->text());
  sessionHandler.setCalibration(&calibrationHandler.calibration);
  sessionHandler.start();
}

void MainWindow::on_stopSessionPushButton_clicked() {
  QTextBrowser* help = new QTextBrowser();
  help->setWindowFlags(Qt::Dialog);
  help->resize(600, 500);

  QFile file(":/help.html");
  file.open(QFile::ReadOnly | QFile::Text);
  QTextStream ReadFile(&file);

  help->setHtml(ReadFile.readAll());
  help->show();
}
