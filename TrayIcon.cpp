#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHostInfo>
#include <qmqtt.h>
#include <QCoreApplication>
#include <QTimer>
//#include<qnetwork.h>
#include <QTime>
#include <QMessageBox>


void MainWindow::showTrayIcon() {
    qDebug() << " showTrayIcon() ";
    trayIcon = new QSystemTrayIcon(this);
    //config_data = "C:\\SENSOR_TEST_DATA\\IMD901\\CONFIG.TXT";
    sModulePath = QCoreApplication::applicationDirPath();
    sModulePath += "\\images\\icon.png";
    //QIcon trayImage(sModulePath);

    QIcon trayImage(":/icons/images/Icon.png");
    trayIcon->setIcon(trayImage);
    trayIcon->setContextMenu(trayIconMenu);
    connect(trayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));
    if(DB_FLAG == 1 && UART_FLAG == 1){
        trayIcon->setToolTip(tr("HANGER V1.2 DB OK"));
    }
    else if(DB_FLAG == 1 && UART_FLAG == 0){
        trayIcon->setToolTip(tr("HANGER V1.2 DB OK"));
    }
    else if(DB_FLAG == 0 && UART_FLAG == 1){
        trayIcon->setToolTip(tr("HANGER V1.2 DB NG!"));
    }
    else if(DB_FLAG == 0 && UART_FLAG == 0){
        trayIcon->setToolTip(tr("HANGER V1.2 DB NG!"));
    }
    else{
        trayIcon->setToolTip(tr("HANGER V1.2 DB NG!"));
    }
    trayIcon->show();
    if(UART_FLAG == 1 && DB_FLAG == 1){
        trayIcon->showMessage(tr("HANGER V1.2"),tr("DB OK, 정상 실행중..."),QSystemTrayIcon::Information,1000);
    }
    else if(UART_FLAG == 1 && DB_FLAG == 0){
        trayIcon->showMessage(tr("HANGER V1.2"),tr("DB NG, DB 접속해주세요!"),QSystemTrayIcon::Information,1000);
    }
    else if(UART_FLAG == 0 && DB_FLAG == 1){
        trayIcon->showMessage(tr("HANGER V1.2"),tr("DB OK"),QSystemTrayIcon::Information,1000);
    }
    else if(UART_FLAG == 0 && DB_FLAG == 0){
        trayIcon->showMessage(tr("HANGER V1.2"),tr("DB NG, DB 접속해주세요!"),QSystemTrayIcon::Information,1000);
    }
    else{
        trayIcon->showMessage(tr("HANGER V1.2"),tr("DB NG, DB 접속해주세요!"),QSystemTrayIcon::Information,1000);
    }

}

void MainWindow::trayActionExecute() {
    QMessageBox::information(this, "TrayIcon", "HANGER V1.2 1890 실행중");
    qDebug() << " trayActionExecute() ";
}

void MainWindow::trayIconActivated(QSystemTrayIcon::ActivationReason reason) {
    qDebug() << " trayIconActivated() ";
    switch (reason) {
        case QSystemTrayIcon::Trigger:
        case QSystemTrayIcon::DoubleClick:
            this->setWindowState(Qt::WindowActive);
            this->show();
            break;
            //this->trayActionExecute();
            //break;
        case QSystemTrayIcon::MiddleClick:
              this->trayActionExecute();
             // this->showMsg();
              break;
        default:
            break;
    }
}


//void MainWindow::showMsg() {
//    qDebug() << " showMsg() ";
//    QMessageBox::information(this, "Tray message", "Hello world!");
//}


void MainWindow::setTrayIconActions() {
    qDebug() << " setTrayIconActions() ";
    launchAction = new QAction("Launch", this);
    quitAction = new QAction("Exit", this);

    //connect (launchAction, SIGNAL(triggered()), this, SLOT(showMsg()));
    connect (launchAction, SIGNAL(triggered()), this, SLOT(trayActionExecute()));
    connect (quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    // Setting system tray's icon menu
    trayIconMenu = new QMenu(this);
    trayIconMenu -> addAction (launchAction);
    trayIconMenu -> addAction (quitAction);
}

void MainWindow::changeEvent(QEvent *event) {
    qDebug() << " changeEvent() ";
    QMainWindow::changeEvent(event);
    if (event->type() == QEvent::WindowStateChange) {
        if (isMinimized()) {
            this->hide();
        }
    }
 }


//void MainWindow::changeEvent(QEvent *e)
//{
//    QMainWindow::changeEvent(e);
//    switch (e->type()) {
//    case QEvent::LanguageChange:
//        ui->retranslateUi(this);
//        break;
//    default:
//        break;
//    }
//}


void MainWindow::closeEvent(QCloseEvent *event) {
    #ifdef Q_OS_OSX
    if (!event->spontaneous() || !isVisible()) {
        return;
    }
    #endif
    if (trayIcon->isVisible()) {
        QMessageBox::information(this, tr("Systray"),
                             tr("The program will keep running in the "
                                "system tray"));
        hide();
        //event->ignore();
        qDebug() << " closeEvent() ";
    }
}


