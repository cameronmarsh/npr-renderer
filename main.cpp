#include "painter.h"
#include "mainwindow.h"
#include <iostream>

#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char *argv[]){
  


  QApplication a(argc, argv);
  QSurfaceFormat format;
  format.setVersion(4,1);
  QSurfaceFormat::setDefaultFormat(format);

  MainWindow w;
  w.show();
  
  return a.exec();
}
