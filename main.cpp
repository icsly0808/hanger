#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHostInfo>
#include <qmqtt.h>
#include <QCoreApplication>
#include <QTimer>
//#include<qnetwork.h>
#include <QTime>
#include <QMessageBox>
#include "qextserialport.h"
#include "qextserialenumerator.h"
#include <QApplication>
#include <QSharedMemory>
#include "LogManager.h"
//#include "chatserver.h"

int main(int argc, char *argv[])
{
    LogManager::getInstance()->initManager();
    QApplication a(argc, argv);
    MainWindow w;
    QSharedMemory shared("HANGER_CLIENT V1.2_1890");


    if(!shared.create(512,QSharedMemory::ReadWrite))
    {
        QMessageBox::information(&w,QObject::tr("HANGER_CLIENT"),QObject::tr("1890_이미 실행중입니다.!"),QMessageBox::Ok);
        exit(0);
    }


    QString sModulePath = QCoreApplication::applicationDirPath();
    sModulePath += "\\images\\icon.png";

    //a.setWindowIcon(QIcon("D:\\images\\icon.png"));
    //a.setWindowIcon(QIcon(sModulePath));
    a.setWindowIcon(QIcon(":/icons/images/Icon.png"));

    a.setQuitOnLastWindowClosed(false);

   // QTimer timer3 = new QTimer(&a);
   // QObject::connect(timer3, SIGNAL(timeout()), &a,SLOT(quit()));


   // printf("Ready\n");


    w.setWindowTitle(QString::fromUtf8("HANGER DBI SW V1.0"));
    w.resize(1080,220);
    //w.setWindowFlags(Qt::WindowMinimizeButtonHint);
    w.show();
    return a.exec();
}
