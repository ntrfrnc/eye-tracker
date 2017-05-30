#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QTextBrowser>
#include <QUrl>
#include <QtSerialPort/QSerialPortInfo>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  setWindowIcon(QIcon(":/app_icon.svg"));
  ui->setupUi(this);

  // Session config
  const auto infos = QSerialPortInfo::availablePorts();
  for (const QSerialPortInfo& info : infos) {
    ui->serialPortComboBox->addItem(info.portName());
  }

  ui->fileNameLineEdit->setText(
      tr("eyetracker_session_%1_%2.csv")
          .arg(QDate::currentDate().toString(Qt::ISODate))
          .arg(sessionHandler.sessionCounter));
  ui->boardBgLineEdit->setText(tr("qrc:/testBoard.html"));

  // Plotter config
  ui->plotOutImagePathLineEdit->setText(
      tr("eyetracker_session_%1_%2.png")
          .arg(QDate::currentDate().toString(Qt::ISODate))
          .arg(sessionHandler.sessionCounter));
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_startCallibrationPushButton_clicked() {
  calibrationHandler.setSerialPort(ui->serialPortNameLineEdit->text());
  calibrationHandler.start();

  if (ui->startSessionOnCalibEndCheckBox->isChecked()) {
    connect(&calibrationHandler, &CalibrationHandler::stopped, this,
            &MainWindow::on_startNewSessionPushButton_clicked);
  } else {
    disconnect(&calibrationHandler, &CalibrationHandler::stopped, this,
               &MainWindow::on_startNewSessionPushButton_clicked);
  }
}

void MainWindow::on_startNewSessionPushButton_clicked() {
  sessionHandler.setFilePath(ui->fileNameLineEdit->text());
  sessionHandler.setBgUrl(QUrl(ui->boardBgLineEdit->text()));
  sessionHandler.setSerialPort(ui->serialPortNameLineEdit->text());
  sessionHandler.setCalibration(&calibrationHandler.calibration);
  sessionHandler.setPointerStatus(ui->showPointerCheckBox->isChecked());
  bool success = sessionHandler.start();

  if (success) {
    ui->fileNameLineEdit->setText(
        tr("eyetracker_session_%1_%2.csv")
            .arg(QDate::currentDate().toString(Qt::ISODate))
            .arg(sessionHandler.sessionCounter));

    ui->plotOutImagePathLineEdit->setText(
        tr("eyetracker_session_%1_%2.png")
            .arg(QDate::currentDate().toString(Qt::ISODate))
            .arg(sessionHandler.sessionCounter));

    ui->plotDataFileLineEdit->setText(ui->fileNameLineEdit->text());

    if (ui->plotDataOnSessionEndCheckBox->isChecked()) {
      connect(&sessionHandler, &SessionHandler::stopped, this,
              &MainWindow::on_plotDataPushButton_clicked);
    } else {
      disconnect(&sessionHandler, &SessionHandler::stopped, this,
                 &MainWindow::on_plotDataPushButton_clicked);
    }
  }
}

void MainWindow::on_plotDataPushButton_clicked() {
  plotter.setOutImageFilePath(ui->plotOutImagePathLineEdit->text());
  plotter.setBgUrl(QUrl(ui->boardBgLineEdit->text()));
  plotter.start(ui->plotDataFileLineEdit->text());
}

void MainWindow::on_showHelpPushButton_clicked() {
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
      this, tr("Save CSV file path"), ui->fileNameLineEdit->text(),
      tr("CSV file (*.csv)"));

  if (fileName != "") {
    ui->fileNameLineEdit->setText(fileName);
  }
}

void MainWindow::on_browseBoardBgPushButton_clicked() {
  QString currentPath = ui->boardBgLineEdit->text().startsWith("qrc:/")
                            ? ""
                            : ui->boardBgLineEdit->text();
  QString fileName = QFileDialog::getOpenFileName(
      this, tr("Choose HTML file"), currentPath, tr("HTML files (*.html)"));
  if (fileName != "") {
    ui->boardBgLineEdit->setText(fileName);
  }
}

void MainWindow::on_serialPortComboBox_currentIndexChanged(
    const QString& arg1) {
  ui->serialPortNameLineEdit->setText(arg1);
}

void MainWindow::on_browsePlotDataFilePushButton_clicked() {
  QString fileName = QFileDialog::getOpenFileName(
      this, tr("Open CSV file"), ui->plotDataFileLineEdit->text(),
      tr("CSV file (*.csv)"));

  if (fileName != "") {
    ui->plotDataFileLineEdit->setText(fileName);
  }
}

void MainWindow::on_browsePlotOutImagePushButton_clicked() {
  QString fileName = QFileDialog::getSaveFileName(
      this, tr("Save PNG file"), ui->plotOutImagePathLineEdit->text(),
      tr("PNG file (*.png)"));

  if (fileName != "") {
    ui->plotOutImagePathLineEdit->setText(fileName);
  }
}
