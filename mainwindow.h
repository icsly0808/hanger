#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <qmqtt.h>
#include <QDebug>
#include <QDialog>
#include <QtNetwork>
#include <QThread>
#include <QNetworkInterface>
#include <QSqlDatabase>
#include <QString>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>
#include <QVariant>
#include <QtSql/QtSql>
#include <qtextstream.h>
#include <QSystemTrayIcon>
#include <QTcpServer>
#include <QTcpSocket>
#include <QSignalMapper>
#include <QList>
#include "qextserialport.h"
#include "qextserialenumerator.h"

using namespace QMQTT;

namespace Ui {
class MainWindow;
}

//class chatServer;
class QextSerialPort;
class QextSerialEnumerator;

//QSystemTrayIcon *TrayIcon;

//class MainWindow : public QMainWindow
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    //explicit MainWindow(QWidget *parent = 0);
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    unsigned char send_com_buf[50];
    QString sModulePath;
    QString C1_COM,S1_COM;
    QString PASSWORD;

    QSqlDatabase db;
    QDate     stCurrentDate;
    QTime stCurrenttime;

    int second;
    int second2;
    int CHKSUM;
    int timer_cnt;
    int IP_COUNT;


QString config_data;
QString log_data;
QString     temp_str;
QString     DB_SERVER;
quint16     DB_PORT;
QString     PORT_NAME;
QString     PORT_BAUD;
QString     PORT_DATA;
QString     PORT_STOP;
QString     PORT_PARITY;
QString     PORT_MODE;
QString     DB_NAME;
QString     DB_USER;
QString     DB_PASS;
QString     ALARM_TABLE;
QString     DATA_TABLE;
QString     MODE_TABLE;
//const QString DB_USER = "lsm";
//const QString DB_PASS = "Test1234!";
//const QString DB_NAME = "HANGER";

QByteArray                      temp_byteC1;
QByteArray                      tcp_byteC1;

int AUTO_UART_FLAG;
int AUTO_DB_FLAG;
int LOG_AUTO;
int DB_SET;
int Tray_Icon;

int ALARM_FLAG;
int MODE_FLAG;
int DATA_FLAG;

int ALARM_R_FLAG_OK;
int MODE_R_FLAG_OK;
int DATA_R_FLAG_OK;

int ALARM_FLAG_OK;
int MODE_FLAG_OK;
int DATA_FLAG_OK;

int ALARM_FLAG_NG;
int MODE_FLAG_NG;
int DATA_FLAG_NG;


int RECV_FLAG;
int ETX_FLAG;
int STX_FLAG;

unsigned char Xor;


unsigned int                    STX_1;
unsigned int                    STX_2;
unsigned int                    STX_3;
unsigned int                    ETX_1;
unsigned int                    ETX_2;

unsigned char                    ack_data[108];
unsigned char                    ack_alarm[15];
unsigned char                    ack_mode[16];
unsigned char                    ack_status[20];

float                    TEMPER;
float                    AMP;
float                    VOLT;
float             CHARGER;
int             BATTERY;
int             SELFTEST;
int             HANGER_NO;
signed int             MODE;
unsigned int             COMM;
unsigned int             COMM_USE;
unsigned int             TIME;
unsigned int             TIME_1;
unsigned int             TIME_2;
unsigned int             TIME_3;

unsigned int                    COM_STATE;
unsigned int                    ALARM;

unsigned int                    YEAR;
unsigned int                    MONTH;
unsigned int                    DATE;
unsigned int                    HOUR;
unsigned int                    MINUTE;
unsigned int                    SECOND;

unsigned int year_int;
unsigned int month_int;
unsigned int day_int;

unsigned int hour_int;
unsigned int minute_int;
unsigned int second_int;

QString     ts_str;
QString     year_str;
QString     month_str;
QString     day_str;
QString     hour_str;
QString     minute_str;
QString     second_str;

int UART_FLAG;
int DB_FLAG;

void delay_ms(int ms);

void open_uart();
void close_uart();
//void showMsg();
void uart_flush();
int recv_cntC1;

void tcpInit();



void send_uart_status();
void send_tcp_status();

void write_log(QString data);

private:
    Ui::MainWindow *ui;

    QMenu *trayIconMenu;
    QAction *launchAction;
    QAction *quitAction;
    QSystemTrayIcon *trayIcon;
    void closeEvent(QCloseEvent *event);

    QTimer *timer;
    QTimer *timer2;

    //chatServer* chatserver;
    QextSerialPort *C_Port1;
    QextSerialEnumerator *enumerator;
    PortSettings csettings;

    int userAmount; //유저 닉네임에 번호를 매김
    QTcpServer tcpServer;
    QList<QTcpSocket*>  *client_list;
    QSet<QTcpSocket*> clients;  //클라이언트 소켓저장
    QMap<QTcpSocket*,QString> users; //유저 이름 저장

    /*
    QTcpServer* tcpServer;
    QTcpSocket* tcpClient;
    QList<QTcpSocket*>  *client_list;
    QSignalMapper *client_mapper;

    QHash<QTcpSocket*, QByteArray*> buffers; //We need a buffer to store data until block has completely received
    QHash<QTcpSocket*, qint32*> sizes; //We need to store the size to verify if a block has received completely
   */

protected:


private slots:
    //void onBaudRateChanged(int idx);
    void readyRead(); //클라이언트로부터 메세지가 전달됨을 감지하는 함수
    void disconnected();  //클라이언트가 접속을 끊을 경우를 감지하는 함수
    void sendDataAll(QByteArray databuf6);
    void send_tcp(QTcpSocket* otherClient, QByteArray databuf5);
    //void incomingConnection(int socketfd);  //처음 유저가 접속을 시도할때 발생하는 함수
    void tcp_ReadingC1(QTcpSocket* otherClient, QByteArray databuf7);
    void onnewConnection();

    void changeEvent(QEvent*);
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void trayActionExecute();
    void setTrayIconActions();
    void showTrayIcon();

    void test_time();
    void test_time2();
    void Data_ReadingC1();
    void on_login_clicked();
    void on_UART_clicked();
    void on_PORT_NAME_currentTextChanged(const QString &arg1);
    void on_DB_NAME_textChanged(const QString &arg1);
    void on_DB_USER_textChanged(const QString &arg1);
    void on_DB_PASS_textChanged(const QString &arg1);


    void on_DB_SERVER_textChanged(const QString &arg1);
    void on_DB_PORT_textChanged(const QString &arg1);
    void on_DB_PASS_returnPressed();

    void on_dataBitsBox_currentIndexChanged(int index);
    void on_stopBitsBox_currentIndexChanged(int index);
    void on_queryModeBox_currentIndexChanged(int index);
    void on_parityBox_currentIndexChanged(int index);
    void on_timeoutBox_valueChanged(int arg1);
    void on_PORT_BAUD_currentIndexChanged(int index);
    void onPortAddedOrRemoved();
    void on_AUTO_DB_stateChanged(int arg1);
    void on_AUTO_UART_stateChanged(int arg1);
    void on_DATA_TABLE_textChanged(const QString &arg1);
    void on_ALARM_TABLE_textChanged(const QString &arg1);
    void on_MODE_TABLE_textChanged(const QString &arg1);
    void on_AUTO_LOG_stateChanged(int arg1);
    void on_AUTO_DB_clicked();
};

#endif // MAINWINDOW_H
