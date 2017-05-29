#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QTextBrowser>
#include <QUrl>
#include <QtSerialPort/QSerialPortInfo>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), sessionCounter(1) {
  setWindowIcon(QIcon(":/app_icon.svg"));
  ui->setupUi(this);

  const auto infos = QSerialPortInfo::availablePorts();
  for (const QSerialPortInfo& info : infos) {
    ui->serialPortComboBox->addItem(info.portName());
  }

  ui->fileNameLineEdit->setText(
      tr("eyetracker_session_%1_%2.csv")
          .arg(QDate::currentDate().toString(Qt::ISODate))
          .arg(sessionCounter));
  ui->boardBgLineEdit->setText(tr("qrc:///testBoard.html"));
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
  bool success = sessionHandler.start();

  if (success) {
    ui->fileNameLineEdit->setText(
        tr("eyetracker_session_%1_%2.csv")
            .arg(QDate::currentDate().toString(Qt::ISODate))
            .arg(++sessionCounter));
  }
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

void MainWindow::on_browseFileNamePushButton_clicked() {
  QString fileName = QFileDialog::getSaveFileName(
      this, tr("Choose CSV file path"), ui->fileNameLineEdit->text(),
      tr("CSV file (*.csv)"));

  if (fileName != "") {
    ui->fileNameLineEdit->setText(fileName);
  }
}

void MainWindow::on_browseBoardBgPushButton_clicked() {
  QString currentPath = ui->boardBgLineEdit->text().startsWith("qrc://")
                            ? ""
                            : ui->boardBgLineEdit->text();
  QString fileName = QFileDialog::getOpenFileName(this, tr("Choose HTML file"), currentPath
                                                  , tr("HTML files (*.html)"));
  if (fileName != "") {
    ui->boardBgLineEdit->setText(fileName);
  }
}

void MainWindow::on_serialPortComboBox_currentIndexChanged(
    const QString& arg1) {
  ui->serialPortNameLineEdit->setText(arg1);
}
