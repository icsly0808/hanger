#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHostInfo>
#include <qmqtt.h>
#include <QCoreApplication>
#include <QTimer>
//#include<qnetwork.h>
#include <QTime>
#include <QMessageBox>
#include <QTcpServer>
#include <QTcpSocket>
#include <QSignalMapper>
#include <QList>
#include "qextserialport.h"
#include "qextserialenumerator.h"
//#include "chatserver.h"
#include "LogManager.h"
//const QString DB_SERVER = "192.168.50.225";
//const quint16 DB_PORT = 3306;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    tcpServer(this)
{
    ui->setupUi(this);
    this->setTrayIconActions();
    this->showTrayIcon();

    connect(LogManager::getInstance(),&LogManager::newLog,
                this,[this](int msgType, const QString &log){
            QString log_text;
            QTextStream stream(&log_text);
            switch (msgType) {
            case QtDebugMsg: stream<<"<span style='color:green;'>"; break;
            case QtInfoMsg: stream<<"<span style='color:blue;'>"; break;
            case QtWarningMsg: stream<<"<span style='color:yellow;'>"; break;
            case QtCriticalMsg: stream<<"<span style='color:red;'>"; break;
            case QtFatalMsg: stream<<"<span style='color:red;'>"; break;
            default: stream<<"<span style='color:red;'>"; break;
            }
            stream<<log<<"</span>";
            //ui->LOG_textEdit->append(log_text);
        });

    //Thread start
    //My_qTh = new qTh(this);
    //connect(My_qTh, SIGNAL(FinishCount(int)), this, SLOT(on_thread_finish(int)));
    //My_qTh->start();

    ui->led_1885->turnOff();
   // ui->led_1886->turnOff();
   // ui->led_1887->turnOff();

    //chatserver=new chatServer();
    //chatserver->start();

    //ui->TCP_R->setPlainText("test");

    // Thread end
    //tcpServer = new QTcpServer;
    tcpInit();


    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
    for (const QHostAddress &address: QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost){
            ui->MyIP->addItem(address.toString());

             qDebug() << address.toString();
        }
    }


    //ui->MyIP->setEditable(true);

    AUTO_UART_FLAG = 0;
    AUTO_DB_FLAG = 0;
    LOG_AUTO = 0;
    DB_SET = 0;
    IP_COUNT = 0;

    DB_SERVER = "DB_SERVER_NAME";
    DB_PORT = 3306;
    DB_NAME = "DB";
    DB_USER = "USER";
    ALARM_TABLE = "hanger_alarm";
    DATA_TABLE = "hanger_data";
    MODE_TABLE = "hanger_mode";
    PORT_NAME = "Comport";
    PORT_BAUD = "BAUD115200";
    PORT_STOP = "1";
    PORT_DATA = "8";
    PORT_PARITY = "NONE";
    PORT_MODE = "EventDriven";

    //ui->AUTO_DB->setChecked(1);
    foreach (QextPortInfo info, QextSerialEnumerator::getPorts())
    //ui->PORT_BAUD->setCurrentText("BAUD115200");
    //ui->PORT_BAUD->setCurrentIndex(0);

    //ui->dataBitsBox->setCurrentIndex(ui->dataBitsBox->findText("8"));

    csettings = {BAUD115200, DATA_8, PAR_NONE, STOP_1, FLOW_OFF, 10};
    C_Port1 = new QextSerialPort("COM1", csettings, QextSerialPort::EventDriven);
    Tray_Icon = 0;
    CHKSUM= 0;
    UART_FLAG = 1;
    DB_FLAG = 0;
    ALARM_FLAG = 0;
    MODE_FLAG = 0;
    DATA_FLAG = 0;

    ALARM_R_FLAG_OK = 0;
    MODE_R_FLAG_OK = 0;
    DATA_R_FLAG_OK = 0;

    DATA_FLAG_OK = 0;
    MODE_FLAG_OK = 0;
    ALARM_FLAG_OK = 0;

    DATA_FLAG_NG = 0;
    MODE_FLAG_NG = 0;
    ALARM_FLAG_NG = 0;

    DB_PASS = "";

    config_data = QCoreApplication::applicationDirPath();
    config_data += "\\images\\CONFIG.TXT";

    qDebug() << "config_data" << config_data;

    //config_data = "C:\\SENSOR_TEST_DATA\\IMD901\\CONFIG.TXT";
    QFile config_file(config_data);
    if(!config_file.open(QIODevice::ReadOnly)) {
       // QMessageBox::information(0, "Create config file", "설정파일을 생성합니다.");
        config_file.close();
        config_file.open(QIODevice::ReadOnly | QIODevice::Text | QIODevice::ReadWrite);
        QTextStream stream(&config_file);
        DB_SERVER = "DB_SERVER_NAME";
        DB_PORT = 3306;
        DB_NAME = "DB";
        DB_USER = "USER";
        PORT_NAME = "Comport";
        PORT_BAUD = "BAUD115200";
        PORT_STOP = "1";
        PORT_DATA = "8";
        PORT_PARITY = "NONE";
        PORT_MODE = "EventDriven";
        ALARM_TABLE = "hanger_alarm";
        DATA_TABLE = "hanger_data";
        MODE_TABLE = "hanger_mode";

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
        ui->DB_SERVER->setText(DB_SERVER);
        ui->DB_PORT->setText(QString::number(DB_PORT));
        ui->DB_NAME->setText(DB_NAME);
        ui->DB_USER->setText(DB_USER);
        ui->DB_PASS->setText(DB_PASS);
        ui->AUTO_DB->setChecked(AUTO_DB_FLAG);
        ui->ALARM_TABLE->setText(ALARM_TABLE);
        ui->MODE_TABLE->setText(MODE_TABLE);
        ui->DATA_TABLE->setText(DATA_TABLE);
    }
    else{
        QTextStream read(&config_file);
        for(int i=0;i<17;i++){
            QString tmp = read.readLine();
            QStringList tmpList = tmp.split(":");
            QString temp;
            switch(i){
                case 0: temp = tmpList[1];
                        DB_SERVER = temp;
                break;

                case 1: temp = tmpList[1];
                        DB_PORT = temp.toInt();
                break;

                case 2: temp = tmpList[1];
                        DB_NAME = temp;
                break;

                case 3: temp = tmpList[1];
                        DB_USER = temp;
                break;

                case 4: temp = tmpList[1];
                        DB_PASS = temp;
                break;

                case 5: temp = tmpList[1];
                        PORT_NAME = temp;
                break;

                case 6: temp = tmpList[1];
                        PORT_BAUD = temp;
                break;

                case 7: temp = tmpList[1];
                        PORT_DATA = temp;
                break;

                case 8: temp = tmpList[1];
                        PORT_STOP = temp;
                break;

                case 9: temp = tmpList[1];
                        PORT_PARITY = temp;
                break;

                case 10: temp = tmpList[1];
                        PORT_MODE = temp;
                break;

                case 11: temp = tmpList[1];
                        AUTO_DB_FLAG = temp.toInt();
                break;

                case 12: temp = tmpList[1];
                        AUTO_UART_FLAG = temp.toInt();
                break;

                case 13: temp = tmpList[1];
                        PORT_NAME = temp;
                break;

                case 14: temp = tmpList[1];
                        ALARM_TABLE = temp;
                break;

                case 15: temp = tmpList[1];
                        DATA_TABLE = temp;
                break;

                case 16: temp = tmpList[1];
                        MODE_TABLE = temp;
                break;


                default:         break;

            }
        }
        config_file.close();
        ui->DB_SERVER->setText(DB_SERVER);
        ui->DB_PORT->setText(QString::number(DB_PORT));
        ui->DB_NAME->setText(DB_NAME);
        ui->DB_USER->setText(DB_USER);
        ui->DB_PASS->setText(DB_PASS);
        ui->ALARM_TABLE->setText(ALARM_TABLE);
        ui->MODE_TABLE->setText(MODE_TABLE);
        ui->DATA_TABLE->setText(DATA_TABLE);
        ui->AUTO_DB->setChecked(AUTO_DB_FLAG);
       // ui->PORT_NAME->setCurrentText(PORT_NAME);
       // ui->DB_PASS->setText("");
    }


    C_Port1->setPortName(PORT_NAME);
    C_Port1->setBaudRate(BAUD115200);
    C_Port1->setParity(PAR_NONE);
    C_Port1->setDataBits(DATA_8);
    C_Port1->setStopBits(STOP_1);
    C_Port1->setFlowControl(FLOW_OFF);
    C_Port1->setTimeout(10);
    C_Port1->setQueryMode(QextSerialPort::EventDriven);

    enumerator = new QextSerialEnumerator(this);
    enumerator->setUpNotifications();   
    //QObject::connect(C_Port1, SIGNAL(readyRead()),SLOT(Data_ReadingC1()));
    // open_uart();

    DB_SERVER = ui->DB_SERVER->text();
    temp_str = ui->DB_PORT->text();
    DB_PORT = temp_str.toInt();
    DB_NAME = ui->DB_NAME->text();
    DB_USER = ui->DB_USER->text();
    ALARM_TABLE = ui->ALARM_TABLE->text();
    DATA_TABLE = ui->DATA_TABLE->text();
    MODE_TABLE = ui->MODE_TABLE->text();

    qDebug() << "DB NAME " << DB_NAME;
    qDebug() << "DB USER " << DB_USER;
    qDebug() << "DB PASS " << DB_PASS;
    qDebug() << "ALARM TABLE " << ALARM_TABLE;
    qDebug() << "DATA TABLE " << DATA_TABLE;
    qDebug() << "MODE TABLE " << MODE_TABLE;

    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(DB_SERVER);
    db.setPort(DB_PORT);

    ui->login->setStyleSheet("QLineEdit {background-color: yellow;}");
    ui->login->setText("DB로그인");
    QString str = "DB";
    str += " 접속 대기";
    ui->DB_OK->setText(str);
    ui->DB_OK->setStyleSheet("QLineEdit {background-color: yellow;}");
   // ui->DB_PASS->setCursorPosition(0);

    connect(enumerator, SIGNAL(deviceDiscovered(QextPortInfo)), SLOT(onPortAddedOrRemoved()));
    connect(enumerator, SIGNAL(deviceRemoved(QextPortInfo)), SLOT(onPortAddedOrRemoved()));

//QThread::msleep(500);
    timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this,SLOT(test_time()));
    //QObject::connect(timer, SIGNAL(clicked()), qApp, SLOT(quit()));

    timer2 = new QTimer(this);
    QObject::connect(timer2, SIGNAL(timeout()), this,SLOT(test_time2()));
    second2 = 0;
    //timer_cnt = 160000;
    timer_cnt = 600000;

    //timer->start(timer_cnt);
   // timer2->start(1000);

    if(AUTO_DB_FLAG == 1){
        on_login_clicked();
    }
    if(AUTO_UART_FLAG == 1){
        on_UART_clicked();
    }

    timer->start(1000);

}

void MainWindow::delay_ms( int millisecondsToWait )
{
    QTime dieTime = QTime::currentTime().addMSecs( millisecondsToWait );
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}

void MainWindow::test_time()
{
    second++;
    //qDebug() <<"Second" << second;
    if(second >= 10){
        send_uart_status();
        second = 0;
    }

}

void MainWindow::test_time2()
{
    second2++;
    qDebug() <<"Second2" << second2;

}



MainWindow::~MainWindow()
{
    if(db.isOpen()) db.close();
    delete ui;
}







void MainWindow::on_AUTO_DB_clicked()
{

}

