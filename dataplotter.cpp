#include "dataplotter.h"

#include <QGuiApplication>
#include <QImage>
#include <QKeyEvent>
#include <QMessageBox>
#include <QPixmap>
#include <QRect>
#include <QScreen>

DataPlotter::DataPlotter() {
  layout.setMargin(0);
  layout.addWidget(&background, 0, 0, 1, 1);
  layout.addWidget(&plotter, 0, 0, 1, 1);
  setLayout(&layout);
}

void DataPlotter::setBgUrl(QUrl url) { this->bgUrl = url; };

void DataPlotter::setOutImageFilePath(QString path) {
  this->outImageFilePath = path;
};

void DataPlotter::start(QString dataFilePath) {
  connect(&background, &QWebEngineView::loadFinished, this,
          &DataPlotter::saveOutputImage);

  background.load(bgUrl);
  background.show();
  showFullScreen();
  plotter.show();

  grabKeyboard();

  plot(dataFilePath);
}

void DataPlotter::stop() {
  background.hide();
  plotter.hide();
  hide();
  releaseKeyboard();
  disconnect(&background, &QWebEngineView::loadFinished, this,
             &DataPlotter::saveOutputImage);
}

void DataPlotter::plot(QString dataFilePath) {
  const uint headerLines = 6;
  qint32 x;
  qint32 y;

  QFile dataFile(dataFilePath);

  if (!dataFile.open(QIODevice::ReadOnly)) {
    errorHandler.showMessage("Can't open file to read data.");
    stop();
    return;
  }

  QTextStream dataStream(&dataFile);

  // Skip header lines
  for (uint i = 0; i < headerLines; i++) {
    dataStream.readLine();
  }

  plotter.clearPoints();

  // Get screen resolution
  QScreen *screen = QGuiApplication::primaryScreen();
  QRect screenGeometry = screen->geometry();
  int screenHeight = screenGeometry.height();

  while (!dataStream.atEnd()) {
    QString line = dataStream.readLine();
    x = line.split(",")[1].toInt();
    y = screenHeight - line.split(",")[2].toInt();

    plotter.addPoint(QPointF(x, y));
  }

  plotter.repaint();
  dataFile.close();
}

void DataPlotter::saveOutputImage() {
  QPixmap pixmap(this->size());
  this->render(&pixmap);

  QFile file(outImageFilePath);

  if (file.exists()) {
    QMessageBox msgBox;
    msgBox.setWindowTitle("File already exists");
    msgBox.setText(tr("File '%1' already exists. Do you want to override it?")
                       .arg(outImageFilePath));
    msgBox.setStandardButtons(QMessageBox::Yes);
    msgBox.addButton(QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    if (msgBox.exec() == QMessageBox::No) {
      return;
    }
  }

  file.open(QIODevice::WriteOnly | QIODevice::Truncate);
  pixmap.save(&file, "PNG");
  file.close();
}

void DataPlotter::keyPressEvent(QKeyEvent *ke) {
  if (ke->isAutoRepeat()) {
    return;
  }

  switch (ke->key()) {
    case Qt::Key_Escape:
      stop();
      break;
  }
}
