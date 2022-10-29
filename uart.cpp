#include "qextserialport.h"
#include "qextserialenumerator.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCore>
#include <QString>
#include <QtGui>
#include <QLabel>
#include <math.h>


void MainWindow::on_UART_clicked()
{
    if (trayIcon->isVisible()) {
        trayIcon->hide();
    }
    //close_uart();

    //csettings = {BAUD115200, DATA_8, PAR_NONE, STOP_1, FLOW_OFF, 10};
    //PortSettings csettings = {BAUD115200, DATA_8, PAR_NONE, STOP_1, FLOW_OFF, 10};
    C_Port1->setPortName(PORT_NAME);
    C_Port1->setParity(PAR_NONE);
    C_Port1->setDataBits(DATA_8);
    C_Port1->setStopBits(STOP_1);
    C_Port1->setFlowControl(FLOW_OFF);
    C_Port1->setTimeout(10);
    C_Port1->setQueryMode(QextSerialPort::EventDriven);

    //C_Port1 = new QextSerialPort(PORT_NAME, csettings, QextSerialPort::EventDriven);

    if(UART_FLAG  == 0) open_uart();
    else close_uart();

    //ui->AUTO_UART->setChecked(1);
    this->setTrayIconActions();
    this->showTrayIcon();
}


void MainWindow::on_PORT_NAME_currentTextChanged(const QString &arg1)
{
    close_uart();

    C_Port1->setPortName(PORT_NAME);
    QFile config_file(config_data);
    //config_file.open(QIODevice::ReadOnly | QIODevice::Text | QIODevice::ReadWrite);
    config_file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream stream(&config_file);
    stream << "DBSERVER:"<< DB_SERVER<<endl;
    stream << "DBPORT:"<< DB_PORT<<endl;
    stream << "DBNAME:"<< DB_NAME<<endl;
    stream << "DBUSER:"<< DB_USER<<endl;
    stream << "DBPASS:"<< DB_PASS<<endl;
    stream << "PORT:"<< PORT_NAME<<endl;
    stream << "BAUD:"<< PORT_BAUD<<endl;
    stream << "DATA:"<< PORT_DATA<<endl;
    stream << "STOP:"<< PORT_STOP<<endl;
    stream << "PARITY:"<< PORT_PARITY<<endl;
    stream << "MODE:"<< PORT_MODE<<endl;
    stream << "AUTODB:"<< AUTO_DB_FLAG<<endl;
    stream << "AUTOUART:"<< AUTO_UART_FLAG<<endl;
    stream << "PORT:"<< PORT_NAME<<endl;
    stream << "ALARM:"<< ALARM_TABLE<<endl;
    stream << "DATA:"<< DATA_TABLE<<endl;
    stream << "MODE:"<< MODE_TABLE<<endl;
    config_file.close();
    qDebug() << "AUTO DB" << AUTO_DB_FLAG;
    qDebug() << "AUTO UART" << AUTO_UART_FLAG;
}

void MainWindow::on_PORT_BAUD_currentIndexChanged(int idx)
{
    close_uart();
    if(idx == 0)  C_Port1->setBaudRate(BAUD115200);
    else if(idx == 1)  C_Port1->setBaudRate(BAUD115200);
    else if(idx == 2)  C_Port1->setBaudRate(BAUD57600);
    else if(idx == 3)  C_Port1->setBaudRate(BAUD19200);
    else if(idx == 4)  C_Port1->setBaudRate(BAUD9600);
    else C_Port1->setBaudRate(BAUD115200);

}



void MainWindow::uart_flush(){
    C_Port1->flush();
}

void MainWindow::open_uart(){

    if(!C_Port1->isOpen()){
        C_Port1->open(QIODevice::ReadWrite);
    }
    if(!C_Port1->isOpen())   {
         UART_FLAG = 0;
         qDebug()<<"C_Port1 ERROR";
         QString str;
         str = PORT_NAME;
         str = str + " 연결실패";
         AUTO_UART_FLAG = 0;
         QFile config_file(config_data);
         //config_file.open(QIODevice::ReadOnly | QIODevice::Text | QIODevice::ReadWrite);
         config_file.open(QIODevice::WriteOnly | QIODevice::Text);
         QTextStream stream(&config_file);
         stream << "DBSERVER:"<< DB_SERVER<<endl;
         stream << "DBPORT:"<< DB_PORT<<endl;
         stream << "DBNAME:"<< DB_NAME<<endl;
         stream << "DBUSER:"<< DB_USER<<endl;
         stream << "DBPASS:"<< DB_PASS<<endl;
         stream << "PORT:"<< PORT_NAME<<endl;
         stream << "BAUD:"<< PORT_BAUD<<endl;
         stream << "DATA:"<< PORT_DATA<<endl;
         stream << "STOP:"<< PORT_STOP<<endl;
         stream << "PARITY:"<< PORT_PARITY<<endl;
         stream << "MODE:"<< PORT_MODE<<endl;
         stream << "AUTODB:"<< AUTO_DB_FLAG<<endl;
         stream << "AUTOUART:"<< AUTO_UART_FLAG<<endl;
         stream << "PORT:"<< PORT_NAME<<endl;
         stream << "ALARM:"<< ALARM_TABLE<<endl;
         stream << "DATA:"<< DATA_TABLE<<endl;
         stream << "MODE:"<< MODE_TABLE<<endl;
         config_file.close();
         qDebug() << "AUTO DB" << AUTO_DB_FLAG;
         qDebug() << "AUTO UART" << AUTO_UART_FLAG;

         QColor col = QColor(Qt::yellow);
         if(col.isValid()) {
            QString qss = QString("background-color: %1").arg(col.name());
         }
     }
    else{
        UART_FLAG = 1;
        qDebug()<<"C_Port1 OPEN";
        QString str;
        str = PORT_NAME;
        str = str + " 연결성공";
        QObject::connect(C_Port1, SIGNAL(readyRead()),SLOT(Data_ReadingC1()));
        QFile config_file(config_data);
        //config_file.open(QIODevice::ReadOnly | QIODevice::Text | QIODevice::ReadWrite);
        config_file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream stream(&config_file);
        stream << "DBSERVER:"<< DB_SERVER<<endl;
        stream << "DBPORT:"<< DB_PORT<<endl;
        stream << "DBNAME:"<< DB_NAME<<endl;
        stream << "DBUSER:"<< DB_USER<<endl;
        stream << "DBPASS:"<< DB_PASS<<endl;
        stream << "PORT:"<< PORT_NAME<<endl;
        stream << "BAUD:"<< PORT_BAUD<<endl;
        stream << "DATA:"<< PORT_DATA<<endl;
        stream << "STOP:"<< PORT_STOP<<endl;
        stream << "PARITY:"<< PORT_PARITY<<endl;
        stream << "MODE:"<< PORT_MODE<<endl;
        stream << "AUTODB:"<< AUTO_DB_FLAG<<endl;
        stream << "AUTOUART:"<< AUTO_UART_FLAG<<endl;
        stream << "PORT:"<< PORT_NAME<<endl;
        stream << "ALARM:"<< ALARM_TABLE<<endl;
        stream << "DATA:"<< DATA_TABLE<<endl;
        stream << "MODE:"<< MODE_TABLE<<endl;
        config_file.close();
        qDebug() << "AUTO DB" << AUTO_DB_FLAG;
        qDebug() << "AUTO UART" << AUTO_UART_FLAG;
        QColor col = QColor(Qt::green);
        if(col.isValid()) {
           QString qss = QString("background-color: %1").arg(col.name());
        }

    }

}

void MainWindow::close_uart(){
    if(C_Port1->isOpen()) C_Port1->close();
    //if(S_Port1->isOpen()) S_Port1->close();
    QString str;
    str = PORT_NAME;
    str = str + " 연결해제됨";
    UART_FLAG = 0;
}

void MainWindow::on_dataBitsBox_currentIndexChanged(int idx)
{

}


void MainWindow::on_stopBitsBox_currentIndexChanged(int idx)
{
}


void MainWindow::on_queryModeBox_currentIndexChanged(int idx)
{
}


void MainWindow::on_parityBox_currentIndexChanged(int idx)
{
}


void MainWindow::on_timeoutBox_valueChanged(int val)
{
    C_Port1->setTimeout(val);
}

void MainWindow::onPortAddedOrRemoved()
{
    //close_uart();
}

void MainWindow::on_AUTO_UART_stateChanged(int arg1)
{

}



/*
//Graph ====================================================================
void MainWindow::Temp_func(float* pArr,int size)
{
     //year/mon/day/hour/minute/sec ---------------------
     QDate d1 = QDate::currentDate();
     int year = d1.year();
     int month = d1.month();
     int day = d1.day();

     QTime t1 = QTime::currentTime();
     int hour = t1.hour();
     int minute = t1.minute();
     int second = t1.second();


}

*/
