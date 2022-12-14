//#include "chatserver.h"
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
#include <QRegExp>
#include <QTextCodec>
#include <QString>
#include <QDebug>
#include <cstring>
#include "qextserialport.h"
#include "qextserialenumerator.h"


void MainWindow::tcpInit(){
    userAmount=-1;
    bool success=tcpServer.listen(QHostAddress::Any,1890);
    if(!success)
    {
        qDebug() << "Could not listen on port 1890.\n";

    }
    else{
        qDebug() << "Server listen on port 1890.\n";
        connect(&tcpServer, SIGNAL(newConnection()), this, SLOT(onnewConnection()));
    }

}

void MainWindow::onnewConnection()
{
    //QTcpSocket* client=new QTcpSocket(this); //클라리언트 소켓 새성
    //client->setSocketDescriptor(socketfd); //소켓드스크립터 설정
    //clients.insert(client); //클라이언트 집합에 새로 들어온 클라이언트 추가

    QTcpSocket *clientSocket = tcpServer.nextPendingConnection();
    clients.insert(clientSocket);

    qDebug() << "New Client from:%s\n" << clientSocket->peerAddress().toString().toLocal8Bit().data();

    connect(clientSocket,SIGNAL(readyRead()),this,SLOT(readyRead()));

    connect(clientSocket,SIGNAL(disconnected()),this,SLOT(disconnected()));
    //_sockets.push_back(clientSocket);
}

void MainWindow::readyRead()
{

   if(IP_COUNT == 0){
        const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
        for (const QHostAddress &address: QNetworkInterface::allAddresses()) {
            if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost){
                ui->MyIP->addItem(address.toString());
                 IP_COUNT++;
                 qDebug() << address.toString();
            }
        }
   }

    QTcpSocket* client=(QTcpSocket*)sender(); //메시지가 들어온 클라이언트를 감지하는 함수 sender

        //QString line=QString::fromUtf8(client->readLine());

        QByteArray Data = client->readAll();
        qDebug() << " Data in: " << Data;
       // ui->TCP_R->setPlainText(QString(Data));

        const std::size_t count = Data.size();
        unsigned char* hex =new unsigned char[count];
        std::memcpy(hex,Data.constData(),count);
        //char* pData = Data.data();

        tcp_byteC1 = QByteArray(reinterpret_cast<char*>(hex),count);
        QString line;
        for(int hh = 0; hh < count; hh++){
           // qDebug() << " pData in: " << pData[hh];
            qDebug() << " hex in: " << hex[hh];
            line += QString::number(hex[hh]);
            line += " ";
        }

        //QRegExp meRegex("^/me:(.*)$");

        if(!users.contains(client)) //새로 들어온 유저면
        {
            QString user=QString("user%1").arg(++userAmount);
            users.insert(client,user);
            qDebug() << QString("new user %1").arg(users[client]).toUtf8();
            send_tcp(client, tcp_byteC1);
            /*
            foreach(QTcpSocket* client,clients)
            {
                QString temp=QString("server:%1 is connneted\n").arg(user);
                qDebug() << clients;
                qDebug() << temp;
                sendDataAll(client, Data);
                //client->write(temp.toUtf8());
                //sendUserList();
            }
            */

        }
        else //이미 있는 유저가 메시지를 보낸경우
        {
           // QString message=line;
            QString user=users[client];
            //printf("User:%s\n",user.toLocal8Bit().data());
            //printf("Message:%s\n",message.toLocal8Bit().data());
            qDebug() << QString("old user %1").arg(users[client]).toUtf8();
            send_tcp(client, tcp_byteC1);
            /*
            foreach(QTcpSocket* otherClient,clients){
                //otherClient->write("DATA OKKKKK");
                qDebug() << QString("RR %1").arg(users[client]).toUtf8();
                //sendDataAll(Data);
            }
            */

        }

}

void MainWindow::disconnected()
{
    QTcpSocket* client=(QTcpSocket*)sender();
    //printf("Client disconnected:%s\n",client->peerAddress().toString().toLocal8Bit().data());

    clients.remove(client);

    QString user=users[client];
    users.remove(client);

    //sendUserList();

    foreach(QTcpSocket* client,clients)
    {
        //client->write(QString("server:"+user+"exit the server.\n").toUtf8());
        qDebug() << QString("server:"+user+"exit the server.\n").toUtf8();
    }


}

void MainWindow::sendDataAll(QByteArray databuf6)
{

    //client->write("rrrr");
    qDebug() << "SEND ALL DATA1";


    /*

    QStringList userList;
    foreach(QString user,users.values())
        userList<<user;
    foreach(QTcpSocket* client,clients){
        //client->write(QString("users:"+ userList.join(",")+"\n").toUtf8());
        client->write(databuf6);
    }

    */

    foreach(QTcpSocket* client,clients){
        client->write(databuf6);
    }

}

void MainWindow::send_tcp(QTcpSocket* otherClient, QByteArray databuf5)
{

    //client->write("rrrr");
    qDebug() << "SEND DATA1";        

    tcp_ReadingC1(otherClient, databuf5);

    /*
    otherClient->write(databuf5);

    const std::size_t count = databuf5.size();
    unsigned char* hex =new unsigned char[count];
    std::memcpy(hex,databuf5.constData(),count);
    QString line;
    for(int hh = 0; hh < count; hh++){
       // qDebug() << " pData in: " << pData[hh];
        qDebug() << " hex in: " << hex[hh];
        line += QString::number(hex[hh]);
        line += " ";
    }

    ui->TCP_S->setPlainText(line);
    */
}

void  MainWindow::tcp_ReadingC1(QTcpSocket* otherClient, QByteArray databuf7)
{
    unsigned int check_sum = 0;
    int count = 0;
    int ii = 0;
    int data_count = 0;
    //int for_int = 0;
    int jj = 0;
    QString DataAsString;
    QByteArray databuf;

    QString ErrorStr;
    ui->led_1885->toggle();


        recv_cntC1 = databuf7.size();


        ETX_FLAG = 0;
        STX_FLAG = 0;
        data_count = 0;
        for(count = 0; count < recv_cntC1; count++){
            data_count++;

            //qDebug() << "databuf7[count] "<< databuf7[count];

            if((databuf7[count] & 0xFF) == 0x16 && (databuf7[count + 1] & 0xFF) == 0x16 && (databuf7[count + 2] & 0xFF) == 0x16){
                STX_FLAG = 1;
                qDebug() << "STX FLAG ";
                data_count = 0;
            }
             if(STX_FLAG == 1){


                 if((databuf7[count] & 0xFF) == 0x0D && (databuf7[count + 1] & 0xFF) == 0x0A){

                     STX_FLAG = 0;
                     ETX_FLAG = 1;
                     qDebug() << "RECV1 COUNT "<< data_count;

                     if(LOG_AUTO == 1){
                         DataAsString = "IP_";
                         DataAsString += otherClient->peerAddress().toString().toLocal8Bit().data();
                         for(jj =0; jj < count ; jj++){
                             DataAsString +="_";
                             DataAsString += QString::number(databuf7[jj]);
                         }
                         write_log(DataAsString);
                     }


                     //if(data_count == 9 || data_count == 11){
                     if(data_count == 9){
                         ALARM_FLAG = 1;
                         qDebug() << "Alarm COUNT "<< count;
                         //for_int = count + 2;
                         for(jj = 0; jj < 11; jj++){
                             ack_alarm[jj] = databuf7[(count - 9) + jj]& 0xFF;;
                             //qDebug() << (count + 1) - jj << "RECV1 ACK DATA "<< ack_com[jj];
                         }
                     }

                     //else if(data_count == 11 || data_count == 13){
                     else if(data_count == 11){
                         MODE_FLAG = 1;
                         qDebug() << "MODE COUNT "<< count;
                         //for_int = count + 2;
                         for(jj = 0; jj < 13; jj++){
                             ack_mode[jj] = databuf7[(count - 11) + jj]& 0xFF;
                             //qDebug() << (count + 1) - jj << "RECV1 INFO DATA "<< ack_info[jj];
                         }
                     }

                     //else if(data_count == 35 || data_count == 37){
                     else if(data_count == 26){
                         DATA_FLAG = 1;
                         qDebug() << "DATA COUNT "<< count;
                         //for_int = count + 2;
                         for(jj = 0; jj < 28; jj++){
                             ack_data[jj] = databuf7[(count - 26) + jj]& 0xFF;
                             //qDebug() << (count + 1) - jj << "RECV1 STATE DATA "<< ack_state[jj];
                         }
                     }

                     else{
                         qDebug() << " Recieve error";
                         break;
                     }


                     data_count = 0;
                 }

             }

        }
        databuf7.clear();

        if(ALARM_FLAG == 1){
            ALARM_FLAG = 0;
            Xor = 0;
            for(int j=0;j<8;j++){
                    Xor = Xor^ack_alarm[j];
            }

            if(Xor == ack_alarm[8]){

                if((ack_alarm[3] & 0xFF) == 0x41){
                    ALARM_FLAG_OK = 1;

                    //ALARM_FLAG_OK = 0;
                    MODE_FLAG_OK = 0;
                    DATA_FLAG_OK = 0;

                    ALARM_R_FLAG_OK = 0;
                    MODE_R_FLAG_OK = 0;
                    DATA_R_FLAG_OK = 0;

                    DATA_FLAG_NG = 0;
                    MODE_FLAG_NG = 0;
                    ALARM_FLAG_NG = 0;

                    MODE = 0x61;
                    check_sum = 1;
                    qDebug() << "ALARM A";
                    //ui->ACK_13->setStyleSheet("QLineEdit {background-color: green;}");
                    //init_value();
                }
                else if((ack_alarm[3] & 0xFF) == 0x52){
                    //ALARM_FLAG_OK = 1;

                    ALARM_FLAG_OK = 0;
                    MODE_FLAG_OK = 0;
                    DATA_FLAG_OK = 0;

                    ALARM_R_FLAG_OK = 1;
                    MODE_R_FLAG_OK = 0;
                    DATA_R_FLAG_OK = 0;

                    DATA_FLAG_NG = 0;
                    MODE_FLAG_NG = 0;
                    ALARM_FLAG_NG = 0;

                    MODE = 0x72;
                    check_sum = 1;
                    qDebug() << "ALARM R";
                    //ui->ACK_13->setStyleSheet("QLineEdit {background-color: green;}");
                    //init_value();
                }
                else{
                    //ui->ACK_13->setStyleSheet("QLineEdit {background-color: red;}");
                    ALARM_FLAG_OK = 0;
                    MODE_FLAG_OK = 0;
                    DATA_FLAG_OK = 0;

                    ALARM_R_FLAG_OK = 0;
                    MODE_R_FLAG_OK = 0;
                    DATA_R_FLAG_OK = 0;

                    DATA_FLAG_NG = 0;
                    MODE_FLAG_NG = 0;
                    ALARM_FLAG_NG = 1;

                    MODE = 0x65;

                    ErrorStr = " Error : 요청값이 다릅니다. COMM ";
                    ErrorStr += "수신값 = ";
                    ErrorStr += QString::number(ack_alarm[3]);
                }

            }
            else{
                //ui->ACK_13->setStyleSheet("QLineEdit {background-color: red;}");
                ALARM_FLAG_OK = 0;
                MODE_FLAG_OK = 0;
                DATA_FLAG_OK = 0;
                ALARM_R_FLAG_OK = 0;
                MODE_R_FLAG_OK = 0;
                DATA_R_FLAG_OK = 0;
                MODE = 0x65;
                qDebug() << "RECV_DATA ALARM CHK ERROR";
                qDebug() << Xor;
                qDebug() << ack_alarm[8];
                ErrorStr = " Error :CHECKSUM 값이 다릅니다. Xor = ";
                ErrorStr += QString::number(Xor);
                ErrorStr += " , 수신값 = ";
                ErrorStr += QString::number(ack_alarm[8]);
            }
        }


        if(MODE_FLAG == 1){
            MODE_FLAG = 0;
            Xor = 0;
            for(int j=0;j<10;j++){
                    Xor = Xor^ack_mode[j];
            }

            if(Xor == ack_mode[10]){
                if((ack_mode[3] & 0xFF) == 0x4D){
                    MODE = 0x6D;
                    MODE_FLAG_OK = 1;
                    ALARM_FLAG_OK = 0;
                    //MODE_FLAG_OK = 0;
                    DATA_FLAG_OK = 0;

                    ALARM_R_FLAG_OK = 0;
                    MODE_R_FLAG_OK = 0;
                    DATA_R_FLAG_OK = 0;

                    DATA_FLAG_NG = 0;
                    MODE_FLAG_NG = 0;
                    ALARM_FLAG_NG = 0;

                    check_sum = 1;
                    qDebug() << "MODE M";
                    //ui->ACK_13->setStyleSheet("QLineEdit {background-color: green;}");
                    //init_value();
                }
                else if((ack_mode[3] & 0xFF) == 0x52){
                    MODE = 0x72;
                    //MODE_FLAG_OK = 0;
                    ALARM_FLAG_OK = 0;
                    MODE_FLAG_OK = 0;
                    DATA_FLAG_OK = 0;

                    ALARM_R_FLAG_OK = 0;
                    MODE_R_FLAG_OK = 1;
                    DATA_R_FLAG_OK = 0;

                    DATA_FLAG_NG = 0;
                    MODE_FLAG_NG = 0;
                    ALARM_FLAG_NG = 0;

                    check_sum = 1;
                    qDebug() << "MODE R";
                    //ui->ACK_13->setStyleSheet("QLineEdit {background-color: green;}");
                    //init_value();
                }
                else{
                    //ui->ACK_13->setStyleSheet("QLineEdit {background-color: red;}");
                    ALARM_FLAG_OK = 0;
                    MODE_FLAG_OK = 0;
                    DATA_FLAG_OK = 0;
                    ALARM_R_FLAG_OK = 0;
                    MODE_R_FLAG_OK = 0;
                    DATA_R_FLAG_OK = 0;

                    DATA_FLAG_NG = 0;
                    MODE_FLAG_NG = 1;
                    ALARM_FLAG_NG = 0;


                    MODE = 0x65;

                    ErrorStr = " Error : 요청값이 다릅니다. COMM ";
                    ErrorStr += "수신값 = ";
                    ErrorStr += QString::number(ack_mode[3]);
                }


            }
            else{
                //ui->ACK_13->setStyleSheet("QLineEdit {background-color: red;}");
                //MODE_FLAG_OK = 0;
                ALARM_FLAG_OK = 0;
                MODE_FLAG_OK = 0;
                DATA_FLAG_OK = 0;
                ALARM_R_FLAG_OK = 0;
                MODE_R_FLAG_OK = 0;
                DATA_R_FLAG_OK = 0;

                DATA_FLAG_NG = 0;
                MODE_FLAG_NG = 0;
                ALARM_FLAG_NG = 0;

                MODE = 0x65;
                qDebug() << "RECV_DATA MODE CHK ERROR";
                qDebug() << Xor;
                qDebug() << ack_mode[10];
                ErrorStr = " Error :CHECKSUM 값이 다릅니다. Xor = ";
                ErrorStr += QString::number(Xor);
                ErrorStr += " , 수신값 = ";
                ErrorStr += QString::number(ack_mode[10]);
            }
        }


        if(DATA_FLAG == 1){
            DATA_FLAG = 0;
            Xor = 0;
            for(int j=0;j<25;j++){
                    Xor = Xor^ack_data[j];
            }

            if(Xor == ack_data[25]){
                if((ack_data[3] & 0xFF) == 0x44){
                    MODE = 0x64;
                    ALARM_FLAG_OK = 0;
                    MODE_FLAG_OK = 0;
                    DATA_FLAG_OK = 1;
                    ALARM_R_FLAG_OK = 0;
                    MODE_R_FLAG_OK = 0;
                    DATA_R_FLAG_OK = 0;

                    DATA_FLAG_NG = 0;
                    MODE_FLAG_NG = 0;
                    ALARM_FLAG_NG = 0;

                    check_sum = 1;
                    qDebug() << "DATA D";
                }
                else if((ack_data[3] & 0xFF) == 0x52){
                    MODE = 0x72;
                    ALARM_FLAG_OK = 0;
                    MODE_FLAG_OK = 0;
                    DATA_FLAG_OK = 0;
                    ALARM_R_FLAG_OK = 0;
                    MODE_R_FLAG_OK = 0;
                    DATA_R_FLAG_OK = 1;

                    DATA_FLAG_NG = 0;
                    MODE_FLAG_NG = 0;
                    ALARM_FLAG_NG = 0;

                    check_sum = 1;
                    qDebug() << "DATA RD";
                }
                else{
                    ALARM_FLAG_OK = 0;
                    MODE_FLAG_OK = 0;
                    DATA_FLAG_OK = 0;
                    ALARM_R_FLAG_OK = 0;
                    MODE_R_FLAG_OK = 0;
                    DATA_R_FLAG_OK = 0;

                    DATA_FLAG_NG = 1;
                    MODE_FLAG_NG = 0;
                    ALARM_FLAG_NG = 0;

                    MODE = 0x65;

                    ErrorStr = " Error : 요청값이 다릅니다. COMM ";
                    ErrorStr += "수신값 = ";
                    ErrorStr += QString::number(ack_data[3]);
                }

            }
            else{
                //ui->ACK_13->setStyleSheet("QLineEdit {background-color: red;}");
                ALARM_FLAG_OK = 0;
                MODE_FLAG_OK = 0;
                DATA_FLAG_OK = 0;
                ALARM_R_FLAG_OK = 0;
                MODE_R_FLAG_OK = 0;
                DATA_R_FLAG_OK = 0;

                DATA_FLAG_NG = 0;
                MODE_FLAG_NG = 0;
                ALARM_FLAG_NG = 0;

                qDebug() << "RECV_DATA DATA CHK ERROR";
                qDebug() << Xor;
                qDebug() << ack_data[34];
                ErrorStr = " Error :CHECKSUM 값이 다릅니다. Xor = ";
                ErrorStr += QString::number(Xor);
                ErrorStr += " , 수신값 = ";
                ErrorStr += QString::number(ack_data[34]);
            }
        }




        QSqlQuery query;
        QString query_text;
        bool result;

        QString format = "yyyy-MM-dd HH:mm:ss";
        QDateTime a = QDateTime::fromString("2020-01-01 00:00:00", format);
        QDateTime b = QDateTime::currentDateTime();

        //qDebug("There are %d seconds to Christmas", (a.secsTo(b))/60);

        QDateTime current = QDateTime::currentDateTime();

        ts_str =  current.toString(format);

        stCurrentDate = QDate::currentDate();

        stCurrenttime = QTime::currentTime();

        year_int = stCurrentDate.year();
        if(year_int >= 2000) year_int = year_int - 2000;
        month_int = stCurrentDate.month();
        day_int = stCurrentDate.day();

        hour_int = stCurrenttime.hour();
        minute_int = stCurrenttime.minute();
        second_int = stCurrenttime.second();

        year_str = QString::number(year_int);
        month_str = QString::number(month_int);
        day_str = QString::number(day_int);
        hour_str = QString::number(hour_int);
        minute_str = QString::number(minute_int);
        second_str = QString::number(second_int);



        if(MODE_FLAG_OK == 1){
            MODE_FLAG_OK = 0;
            //HANGER_NO = 4;
            //COMM = 0;
            //TIME = 0;


            HANGER_NO = ack_mode[4] * 256 + ack_mode[5];
            COMM =  ack_mode[6];
            TIME = ack_mode[7] * 65536 + ack_mode[8]*256 + ack_mode[9];

            query_text = "select Comm,Time from ";
            query_text += MODE_TABLE;
            query_text += " where Hanger_No = '";
            query_text += QString::number(HANGER_NO);
            query_text += "'";
            qDebug() << "0000 SETUP11 Query_text!" << query_text;
            result = query.prepare(query_text);
            int db_count = 0;
            if(result){
                //query.exec(query_text);

                if (!query.exec(query_text)) {
                  qDebug() << "ERROR1";
                  ErrorStr = " Error : 행거번호 ";
                  ErrorStr += QString::number(HANGER_NO);
                  ErrorStr += " 의 MODE 데이터가 존재하지 않습니다. ERROR CODE 0x65를 전송합니다.";
                  send_com_buf[3] = 0x65; //e
                  COMM =  ack_mode[6];
                  TIME = ack_mode[7] * 65536 + ack_mode[8]*256 + ack_mode[9];

                } else {
                  while (query.next()) {
                   // COMM = query.value(0).toInt();
                   // TIME = query.value(1).toInt();
                    qDebug() << " COM : "<< COMM << "TIME :"<< TIME;
                  }

                }

                db_count = query.size();
                if(db_count == 0) {
                    qDebug() << "BB00 SETUP11 Query_text OK!";
                    result = 0;
                }

            }
            else{
                qDebug() << "AA00 SETUP11 Query_text ERROR!";
                if(DB_SET == 1) on_login_clicked();
            }


            if(result){
                qDebug() << "RESULT UPDATE";
                send_com_buf[3] = 0x6D;
                ErrorStr = " OK : 행거번호 ";
                ErrorStr += QString::number(HANGER_NO);
                ErrorStr += " 의 MODE 데이터를 UPDATE 합니다. UPDATE CODE 0x6D를 전송합니다.";
            }
            else{
                qDebug() << "RESULT ERROR";
                send_com_buf[3] = 0x69; //i
                ErrorStr = " INSERT : 행거번호 ";
                ErrorStr += QString::number(HANGER_NO);
                ErrorStr += " 의 MODE 데이터가 존재하지 않습니다. INSERT CODE 0x69를 전송합니다.";
            }
                query_text = "INSERT INTO ";
                query_text += MODE_TABLE;
                query_text += " SET ";
                query_text = query_text + " Hanger_No = '" + QString::number(HANGER_NO) + "'";
                query_text = query_text + ", Comm = '" + QString::number(COMM) + "'";
                query_text = query_text + ", Time = '" + QString::number(TIME) + "'";
                query_text = query_text + " ON DUPLICATE KEY UPDATE ";
                query_text = query_text + " Comm = '" + QString::number(COMM) + "'";
                query_text = query_text + ", Time = '" + QString::number(TIME) + "'";

                qDebug()<<"query_text"<<query_text;

                bool result2 = query.prepare(query_text);
                 if (!result2) {
                    qDebug() << query.lastError();
                    ErrorStr = " DB 접속 ERROR :";
                    ErrorStr += query.lastError().text();
                }

                 if(!query.exec(query_text))
                 {
                       qDebug() << "111Can't Execute Query_text !"<< query.lastError();
                 }
                 else
                 {
                    qDebug() << "111TABLE Query_text OK!";
                 }



                Xor = 0;
                send_com_buf[0] = 0x16;
                send_com_buf[1] = 0x16;
                send_com_buf[2] = 0x16;
                //send_com_buf[3] = 0x6D;
                send_com_buf[4] = ack_mode[4];
                send_com_buf[5] = ack_mode[5];

                send_com_buf[6] = COMM;
                TIME_1 = TIME / 65536;
                TIME_2 = (TIME % 65536)/256;
                TIME_3 = TIME % 256;
                send_com_buf[7] = TIME_1;
                send_com_buf[8] = TIME_2;
                send_com_buf[9] = TIME_3;
                for(ii =0; ii < 10 ; ii++){
                    Xor = Xor^send_com_buf[ii];
                }
                send_com_buf[10] = Xor; //CHKSUM
                send_com_buf[11] = 0x0D;
                send_com_buf[12] = 0x0A;

                    databuf = QByteArray(reinterpret_cast<char*>(send_com_buf),13);
                    otherClient->write(databuf);

                    const std::size_t count = databuf.size();
                    unsigned char* hex =new unsigned char[count];
                    std::memcpy(hex,databuf.constData(),count);
                    QString line;
                    for(int hh = 0; hh < count; hh++){
                       // qDebug() << " pData in: " << pData[hh];
                        qDebug() << " hex in: " << hex[hh];
                        line += QString::number(hex[hh]);
                        line += " ";
                    }
                    databuf.clear();

        }




        if(MODE_R_FLAG_OK == 1){
            MODE_R_FLAG_OK = 0;


            HANGER_NO = ack_mode[4] * 256 + ack_mode[5];
            COMM =  ack_mode[6];
            TIME = ack_mode[7] * 65536 + ack_mode[8]*256 + ack_mode[9];

            query_text = "select Comm,Time from ";
            query_text += MODE_TABLE;
            query_text += " where Hanger_No = '";
            query_text += QString::number(HANGER_NO);
            query_text += "'";
            qDebug() << "0000 SETUP11 Query_text!" << query_text;
            result = query.prepare(query_text);
            int db_count = 0;
            if(result){
                //query.exec(query_text);

                if (!query.exec(query_text)) {
                  qDebug() << "ERROR1";
                  ErrorStr = " Error : 행거번호 ";
                  ErrorStr += QString::number(HANGER_NO);
                  ErrorStr += " 의 MODE 데이터가 존재하지 않습니다. ERROR CODE 0x65를 전송합니다.";
                  send_com_buf[3] = 0x65; //e
                  COMM =  ack_mode[6];
                  TIME = ack_mode[7] * 65536 + ack_mode[8]*256 + ack_mode[9];

                } else {
                  while (query.next()) {
                    COMM = query.value(0).toInt();
                    TIME = query.value(1).toInt();
                    qDebug() << " COM : "<< COMM << "TIME :"<< TIME;
                  }

                }

                db_count = query.size();
                if(db_count == 0) {
                    qDebug() << "BB00 SETUP11 Query_text OK!";
                    result = 0;
                }

            }
            else{
                qDebug() << "AA00 SETUP11 Query_text ERROR!";
                if(DB_SET == 1) on_login_clicked();
            }



            if(result){
                qDebug() << "RESULT MODE DATA 전송";
                send_com_buf[3] = 0x72;
                ErrorStr = " OK : 행거번호 ";
                ErrorStr += QString::number(HANGER_NO);
                ErrorStr += " 의 MODE 데이터를 전송 합니다. 응답코드 0x72를 전송합니다.";
            }
            else{
                qDebug() << "RESULT MODE DATA ERROR";
                send_com_buf[3] = 0x65;
                ErrorStr = " OK : 행거번호 ";
                ErrorStr += QString::number(HANGER_NO);
                ErrorStr += " 의 MODE 데이터가 존재하지 않습니다. ERROR CODE 0x65를 전송합니다.";
                COMM = 0;
                TIME = 0;

            }


                Xor = 0;
                send_com_buf[0] = 0x16;
                send_com_buf[1] = 0x16;
                send_com_buf[2] = 0x16;
                //send_com_buf[3] = 0x6D;
                send_com_buf[4] = ack_mode[4];
                send_com_buf[5] = ack_mode[5];

                send_com_buf[6] = COMM;
                TIME_1 = TIME / 65536;
                TIME_2 = (TIME % 65536)/256;
                TIME_3 = TIME % 256;
                send_com_buf[7] = TIME_1;
                send_com_buf[8] = TIME_2;
                send_com_buf[9] = TIME_3;
                for(ii =0; ii < 10 ; ii++){
                    Xor = Xor^send_com_buf[ii];
                }
                send_com_buf[10] = Xor; //CHKSUM
                send_com_buf[11] = 0x0D;
                send_com_buf[12] = 0x0A;

                //open_uart();

                    databuf = QByteArray(reinterpret_cast<char*>(send_com_buf),13);
                    otherClient->write(databuf);


                    const std::size_t count = databuf.size();
                    unsigned char* hex =new unsigned char[count];
                    std::memcpy(hex,databuf.constData(),count);
                    QString line;
                    for(int hh = 0; hh < count; hh++){
                       // qDebug() << " pData in: " << pData[hh];
                        qDebug() << " hex in: " << hex[hh];
                        line += QString::number(hex[hh]);
                        line += " ";
                    }
                    databuf.clear();

        }







        if(ALARM_FLAG_OK == 1){
            ALARM_FLAG_OK = 0;

            //HANGER_NO = 4;
            //COMM = 0;
            //TIME = 0;


            HANGER_NO = ack_alarm[4] * 256 + ack_alarm[5];
            BATTERY =  ack_alarm[6];
            SELFTEST =  ack_alarm[7];

            query_text = "select * from ";
            query_text += ALARM_TABLE;
            query_text += " where Hanger_No = '";
            query_text += QString::number(HANGER_NO);
            query_text += "'";
            qDebug() << "0000 SETUP11 Query_text!" << query_text;
            result = query.prepare(query_text);
            int db_count = 0;
            if(result){
                query.exec(query_text);
                db_count = query.size();
                if(db_count == 0) {
                    qDebug() << "0000 SETUP11 Query_text OK!";
                    result = 0;
                }
            }
            else{
                qDebug() << "AA00 SETUP11 Query_text ERROR!";
                if(DB_SET == 1) on_login_clicked();
            }

            if(result){
                send_com_buf[3] = 0x61;
                qDebug() << "RESULT ";
                ErrorStr = " OK : 행거번호 ";
                ErrorStr += QString::number(HANGER_NO);
                ErrorStr += " 의 ALARM을 DATA를 전송합니다. UPDATE CODE 0x61를 전송합니다.";
            }
            else{
                send_com_buf[3] = 0x69;
                qDebug() << "RESULT ";
                ErrorStr = " ERROR : 행거번호 ";
                ErrorStr += QString::number(HANGER_NO);
                ErrorStr += " 의 ALARM DATA가 없습니다. INSERT CODE 0x69를 전송합니다.";
            }

            query_text = "INSERT INTO ";
            query_text += ALARM_TABLE;
            query_text += " SET ";
            query_text = query_text + " Hanger_No = '" + QString::number(HANGER_NO) + "'";
            query_text = query_text + ", Battery = '" + QString::number(BATTERY) + "'";
            query_text = query_text + ", SelfTest = '" + QString::number(SELFTEST) + "'";
            query_text = query_text + " ON DUPLICATE KEY UPDATE ";
            query_text = query_text + " Battery = '" + QString::number(BATTERY) + "'";
            query_text = query_text + ", SelfTest = '" + QString::number(SELFTEST) + "'";

            qDebug()<<"query_text"<<query_text;

            bool result2 = query.prepare(query_text);
             if (!result2) {
                qDebug() << query.lastError();
                ErrorStr = " DB 접속 ERROR :";
                ErrorStr += query.lastError().text();
            }

             if(!query.exec(query_text))
             {
                   qDebug() << "111Can't Execute Query_text !"<< query.lastError();
             }
             else
             {
                qDebug() << "111TABLE Query_text OK!";
             }



                 Xor = 0;
                 send_com_buf[0] = 0x16;
                 send_com_buf[1] = 0x16;
                 send_com_buf[2] = 0x16;
                 //send_com_buf[3] = 0x61;
                 send_com_buf[4] = ack_alarm[4];
                 send_com_buf[5] = ack_alarm[5];
                 send_com_buf[6] = ack_alarm[6];
                 send_com_buf[7] = ack_alarm[7];
                 for(ii =0; ii < 8 ; ii++){
                     Xor = Xor^send_com_buf[ii];
                 }
                 send_com_buf[8] = Xor; //CHKSUM
                 send_com_buf[9] = 0x0D;
                 send_com_buf[10] = 0x0A;

                     databuf = QByteArray(reinterpret_cast<char*>(send_com_buf),11);
                     otherClient->write(databuf);

                     const std::size_t count = databuf.size();
                     unsigned char* hex =new unsigned char[count];
                     std::memcpy(hex,databuf.constData(),count);
                     QString line;
                     for(int hh = 0; hh < count; hh++){
                        // qDebug() << " pData in: " << pData[hh];
                         qDebug() << " hex in: " << hex[hh];
                         line += QString::number(hex[hh]);
                         line += " ";
                     }
                     databuf.clear();


        }



        if(ALARM_R_FLAG_OK == 1){
            ALARM_R_FLAG_OK = 0;

            //HANGER_NO = 4;
            //COMM = 0;
            //TIME = 0;


            HANGER_NO = ack_alarm[4] * 256 + ack_alarm[5];
            BATTERY =  ack_alarm[6];
            SELFTEST =  ack_alarm[7];
            //query_text = "select * from alarm where Hanger_No = '";
            query_text = "select Battery,SelfTest from ";
            query_text += ALARM_TABLE;
            query_text += " where Hanger_No = '";
            query_text += QString::number(HANGER_NO);
            query_text += "'";
            qDebug() << "0000 SETUP11 Query_text!" << query_text;
            result = query.prepare(query_text);
            int db_count = 0;
            if(result){
//                query.exec(query_text);

                if (!query.exec(query_text)) {
                  qDebug() << "ERROR1";
                  ErrorStr = " Error : 행거번호 ";
                  ErrorStr += QString::number(HANGER_NO);
                  ErrorStr += " 의 ALARM 데이터가 존재하지 않습니다. ERROR CODE 0x65를 전송합니다.";
                  send_com_buf[3] = 0x65; //e
                  BATTERY =  0;
                  SELFTEST = 0;

                } else {
                  while (query.next()) {
                    BATTERY = query.value(0).toInt();
                    SELFTEST = query.value(1).toInt();
                    qDebug() << " COM : "<< COMM << "TIME :"<< TIME;
                  }

                }

                db_count = query.size();
                if(db_count == 0) {
                    qDebug() << "0000 SETUP11 Query_text OK!";
                    result = 0;
                }
            }
            else{
                qDebug() << "AA00 SETUP11 Query_text ERROR!";
                if(DB_SET == 1) on_login_clicked();
            }

            if(result){
                send_com_buf[3] = 0x72;
                send_com_buf[6] = BATTERY;
                send_com_buf[7] = SELFTEST;
                qDebug() << "RESULT";
                ErrorStr = " OK : 행거번호 ";
                ErrorStr += QString::number(HANGER_NO);
                ErrorStr += " 의 ALARM DATA를 전송 합니다. 응답코드 0x72를 전송합니다.";
            }
            else{
                send_com_buf[3] = 0x65;
                send_com_buf[6] = 0;
                send_com_buf[7] = 0;
                qDebug() << "RESULT";
                ErrorStr = " ERROR : 행거번호 ";
                ErrorStr += QString::number(HANGER_NO);
                ErrorStr += " 의 ALARM DATA가 없습니다. ERROR CODE 0x65를 전송합니다.";
            }


                 Xor = 0;
                 send_com_buf[0] = 0x16;
                 send_com_buf[1] = 0x16;
                 send_com_buf[2] = 0x16;
                 //send_com_buf[3] = 0x61;
                 send_com_buf[4] = ack_alarm[4];
                 send_com_buf[5] = ack_alarm[5];
                 //send_com_buf[6] = ack_alarm[6];
                 //send_com_buf[7] = ack_alarm[7];


                 for(ii =0; ii < 8 ; ii++){
                     Xor = Xor^send_com_buf[ii];
                 }
                 send_com_buf[8] = Xor; //CHKSUM
                 send_com_buf[9] = 0x0D;
                 send_com_buf[10] = 0x0A;

                 //open_uart();

                     databuf = QByteArray(reinterpret_cast<char*>(send_com_buf),11);
                     otherClient->write(databuf);

                     const std::size_t count = databuf.size();
                     unsigned char* hex =new unsigned char[count];
                     std::memcpy(hex,databuf.constData(),count);
                     QString line;
                     for(int hh = 0; hh < count; hh++){
                        // qDebug() << " pData in: " << pData[hh];
                         qDebug() << " hex in: " << hex[hh];
                         line += QString::number(hex[hh]);
                         line += " ";
                     }
                     databuf.clear();

        }






        if(DATA_FLAG_OK == 1){
            DATA_FLAG_OK = 0;
            //HANGER_NO = 4;
            //COMM = 0;
            //TIME = 0;

            /*
            ui->DATA_29->setText(QString::number(ack_data[28]));
            ui->DATA_30->setText(QString::number(ack_data[29]));
            ui->DATA_31->setText(QString::number(ack_data[30]));
            ui->DATA_32->setText(QString::number(ack_data[31]));
            ui->DATA_33->setText(QString::number(ack_data[32]));
            ui->DATA_34->setText(QString::number(ack_data[33]));
            ui->DATA_35->setText(QString::number(ack_data[34]));
            ui->DATA_36->setText(QString::number(ack_data[35]));
            ui->DATA_37->setText(QString::number(ack_data[36]));
            */

            float temp_fl = 0.0;
            HANGER_NO = ack_data[4] * 256 + ack_data[5];
            AMP = 0.0;
            VOLT = 0.0;
            TEMPER = 0.0;
            CHARGER = 0.0;

            //temp_fl = 0.1 * (ack_data[15]*65536 + ack_data[16]*256 + ack_data[17]);
            AMP = (ack_data[12]*65536 + ack_data[13]*256 + ack_data[14])/100.0;

            //temp_fl = 0.1 * (ack_data[21]*65536 + ack_data[22]*256 + ack_data[23]);
            //VOLT = ack_data[18]*65536 + ack_data[19]*256 + ack_data[20] + temp_fl;
            VOLT = (ack_data[15]*65536 + ack_data[16]*256 + ack_data[17])/100.0;

            CHARGER = (ack_data[18]*256 + ack_data[19])/10.0;

            //temp_fl = 0.1 * (ack_data[29]*65536 + ack_data[30]*256 + ack_data[31]);
            //TEMPER = ack_data[26]*65536 + ack_data[27]*256 + ack_data[28] + temp_fl;
            TEMPER = (ack_data[20]*65536 + ack_data[21]*256 + ack_data[22])/100.0;

            COMM = ack_data[23];

            COMM_USE = ack_data[24];




            query_text = "select * from ";
            query_text += DATA_TABLE;
            query_text += " where Hanger_No = '";
            query_text += QString::number(HANGER_NO);
            query_text += "'";
            qDebug() << "0000 SETUP11 Query_text!" << query_text;
            result = query.prepare(query_text);
            int db_count = 0;
            if(result){
                query.exec(query_text);
                db_count = query.size();
                if(db_count == 0) {
                    qDebug() << "0000 SETUP11 Query_text OK!";
                    result = 0;
                }
            }
            else{
                qDebug() << "AA00 SETUP11 Query_text ERROR!";
                if(DB_SET == 1) on_login_clicked();
            }

            if(result){
                send_com_buf[3] = 0x64;
                qDebug() << "RESULT UPDATE";
                ErrorStr = " OK Update: 행거번호 ";
                ErrorStr += QString::number(HANGER_NO);
                ErrorStr += " 의 DATA 업데이트 합니다.UPDATE CODE 0x64를 전송합니다.";
            }
            else{
                send_com_buf[3] = 0x69;
                qDebug() << "RESULT INSERT";
                ErrorStr = " OK New Insert: 행거번호 ";
                ErrorStr += QString::number(HANGER_NO);
                ErrorStr += " 의 DATA를 신규 등록합니다. INSERT CODE 0x69를 전송합니다.";
            }

            query_text = "INSERT INTO ";
            query_text += DATA_TABLE;
            query_text += " SET ";
            query_text = query_text + " Hanger_No = '" + QString::number(HANGER_NO) + "'";
            query_text = query_text + ", UpdateTime = '" + ts_str + "'";
            query_text = query_text + ", Amp = '" + QString::number(AMP) + "'";
            query_text = query_text + ", Bat_Vot = '" + QString::number(VOLT) + "'";
            query_text = query_text + ", Bat_Level = '" + QString::number(CHARGER) + "'";
            query_text = query_text + ", Temp = '" + QString::number(TEMPER) + "'";
            query_text = query_text + ", Comm = '" + QString::number(COMM) + "'";
            query_text = query_text + ", Charge = '" + QString::number(COMM_USE) + "'";
            query_text = query_text + " ON DUPLICATE KEY UPDATE ";
            query_text = query_text + " UpdateTime = '" + ts_str + "'";
            query_text = query_text + ", Amp = '" + QString::number(AMP) + "'";
            query_text = query_text + ", Bat_Vot = '" + QString::number(VOLT) + "'";
            query_text = query_text + ", Bat_Level = '" + QString::number(CHARGER) + "'";
            query_text = query_text + ", Temp = '" + QString::number(TEMPER) + "'";
            query_text = query_text + ", Comm = '" + QString::number(COMM) + "'";
            query_text = query_text + ", Charge = '" + QString::number(COMM_USE) + "'";

            qDebug()<<"query_text"<<query_text;

            bool result2 = query.prepare(query_text);
             if (!result2) {
                //qDebug() << query.lastError();
                ErrorStr = " DB 접속 ERROR :";
                ErrorStr += query.lastError().text();
            }

             if(!query.exec(query_text))
             {
                   qDebug() << "111Can't Execute Query_text !"<< query.lastError();
             }
             else
             {
                qDebug() << "111TABLE Query_text OK!";
             }


                 Xor = 0;
                 send_com_buf[0] = 0x16;
                 send_com_buf[1] = 0x16;
                 send_com_buf[2] = 0x16;
                 //send_com_buf[3] = 0x64;
                 send_com_buf[4] = ack_data[4];
                 send_com_buf[5] = ack_data[5];
                 send_com_buf[6] = year_int;
                 send_com_buf[7] = month_int;
                 send_com_buf[8] = day_int;
                 send_com_buf[9] = hour_int;
                 send_com_buf[10] = minute_int;
                 send_com_buf[11] = second_int;
                 send_com_buf[12] = ack_data[12];
                 send_com_buf[13] = ack_data[13];
                 send_com_buf[14] = ack_data[14];
                 send_com_buf[15] = ack_data[15];
                 send_com_buf[16] = ack_data[16];
                 send_com_buf[17] = ack_data[17];
                 send_com_buf[18] = ack_data[18];
                 send_com_buf[19] = ack_data[19];
                 send_com_buf[20] = ack_data[20];
                 send_com_buf[21] = ack_data[21];
                 send_com_buf[22] = ack_data[22];
                 send_com_buf[23] = ack_data[23];
                 send_com_buf[24] = ack_data[24];

                 for(ii =0; ii < 25 ; ii++){
                     Xor = Xor^send_com_buf[ii];
                 }
                 send_com_buf[25] = Xor; //CHKSUM
                 send_com_buf[26] = 0x0D;
                 send_com_buf[27] = 0x0A;

                     databuf = QByteArray(reinterpret_cast<char*>(send_com_buf),28);
                     otherClient->write(databuf);

                     const std::size_t count = databuf.size();
                     unsigned char* hex =new unsigned char[count];
                     std::memcpy(hex,databuf.constData(),count);
                     QString line;
                     for(int hh = 0; hh < count; hh++){
                        // qDebug() << " pData in: " << pData[hh];
                         qDebug() << " hex in: " << hex[hh];
                         line += QString::number(hex[hh]);
                         line += " ";
                     }
                     databuf.clear();


        }



        if(DATA_R_FLAG_OK == 1){
            DATA_R_FLAG_OK = 0;
            QString u_time = "2000-01-01 12:00:00";



            float temp_fl = 0.0;
            HANGER_NO = ack_data[4] * 256 + ack_data[5];
            AMP = 0.0;
            VOLT = 0.0;
            TEMPER = 0.0;
            CHARGER = 0.0;

            AMP = (ack_data[12]*65536 + ack_data[13]*256 + ack_data[14])/100.0;

            VOLT = (ack_data[15]*65536 + ack_data[16]*256 + ack_data[17])/100.0;

            CHARGER = (ack_data[18]*256 + ack_data[19])/10.0;

            TEMPER = (ack_data[20]*65536 + ack_data[21]*256 + ack_data[22])/100.0;

            COMM = ack_data[23];

            COMM_USE = ack_data[24];

            query_text = "select UpdateTime, Amp, Bat_Vot, Bat_Level, Temp, Comm, Charge from ";
            query_text += DATA_TABLE;
            query_text += " where Hanger_No = '";
            query_text += QString::number(HANGER_NO);
            query_text += "'";
            qDebug() << "0000 SETUP11 Query_text!" << query_text;
            result = query.prepare(query_text);
            int db_count = 0;
            if(result){
               // query.exec(query_text);
                if (!query.exec(query_text)) {
                  qDebug() << "ERROR1";
                  ErrorStr = " Error : 행거번호 ";
                  ErrorStr += QString::number(HANGER_NO);
                  ErrorStr += " 의 ALARM 데이터가 존재하지 않습니다. ERROR CODE 0x65를 전송합니다.";
                  send_com_buf[3] = 0x65; //e

                  AMP = 0;
                  VOLT = 0;
                  CHARGER = 0;
                  TEMPER = 0;
                  COMM = 0;
                  COMM_USE = 0;

                } else {
                  while (query.next()) {
                    u_time = query.value(0).toString();
                    send_com_buf[3] = 0x72; //r
                    AMP = query.value(1).toFloat() * 100;
                    VOLT = query.value(2).toFloat() * 100;
                    CHARGER = query.value(3).toFloat() * 10;
                    TEMPER = query.value(4).toFloat() * 100;
                    COMM = query.value(5).toInt();
                    COMM_USE = query.value(6).toInt();
                    //qDebug() << " COM : "<< COMM << "TIME :"<< TIME;
                  }

                }



                db_count = query.size();
                if(db_count == 0) {
                    qDebug() << "0000 SETUP11 Query_text OK!";
                    result = 0;
                }
            }
            else{
                qDebug() << "AA00 SETUP11 Query_text ERROR!";
                if(DB_SET == 1) on_login_clicked();
            }


            if(result){

                QDateTime DBUTime = QDateTime::fromString(u_time,Qt::ISODate);
                QDate  DBDate = QDate::fromString(u_time,Qt::ISODate);
                year_int = DBDate.year();
                if(year_int >= 2000) year_int = year_int - 2000;
                month_int = DBDate.month();
                day_int = DBDate.day();

                hour_int = DBUTime.time().hour();
                minute_int = DBUTime.time().minute();
                second_int = DBUTime.time().second();

                year_str = QString::number(year_int);
                month_str = QString::number(month_int);
                day_str = QString::number(day_int);
                hour_str = QString::number(hour_int);
                minute_str = QString::number(minute_int);
                second_str = QString::number(second_int);

                qDebug() << "YEAR STR " << year_str;
                qDebug() << "HOUR STR " << hour_str;
                qDebug() << "AMP " << AMP;


                qDebug() << "RESULT";
                ErrorStr = " OK : 행거번호 ";
                ErrorStr += QString::number(HANGER_NO);
                ErrorStr += " 의 DATA를 전송 합니다. 전송코드 0x72를 전송합니다.";


                send_com_buf[3] = 0x72;
                TIME = AMP;
                TIME_1 = TIME / 65536;
                TIME_2 = (TIME % 65536)/256;
                TIME_3 = TIME % 256;
                send_com_buf[12] = TIME_1;  //AMP
                send_com_buf[13] = TIME_2;  //AMP
                send_com_buf[14] = TIME_3;  //AMP

                TIME = VOLT;
                TIME_1 = TIME / 65536;
                TIME_2 = (TIME % 65536)/256;
                TIME_3 = TIME % 256;
                send_com_buf[15] = TIME_1;  //VOLt
                send_com_buf[16] = TIME_2;  //VOLT
                send_com_buf[17] = TIME_3;  //VOLT

                TIME = CHARGER;
                TIME_1 = TIME / 65536;
                TIME_2 = (TIME % 65536)/256;
                TIME_3 = TIME % 256;
                send_com_buf[18] = TIME_2;  //CHARGE
                send_com_buf[19] = TIME_3;  //CHARGE

                TIME = TEMPER;
                TIME_1 = TIME / 65536;
                TIME_2 = (TIME % 65536)/256;
                TIME_3 = TIME % 256;
                send_com_buf[20] = TIME_1;  //TEMP
                send_com_buf[21] = TIME_2;  //TEMP
                send_com_buf[22] = TIME_3;  //TEMP
                send_com_buf[23] = COMM;  //COMM
                send_com_buf[24] = COMM_USE;  //COME_USE



            }
            else{

                qDebug() << "RESULT";
                ErrorStr = " ERROR : 행거번호 ";
                ErrorStr += QString::number(HANGER_NO);
                ErrorStr += " 의 DATA가 없습니다. ERROR CODE 0x65를 전송합니다.";

                year_int = 0;
                month_int = 0;
                day_int = 0;

                hour_int = 0;
                minute_int = 0;
                second_int = 0;

                send_com_buf[3] = 0x65;

                send_com_buf[12] = 0;  //AMP
                send_com_buf[13] = 0;  //AMP
                send_com_buf[14] = 0;  //AMP
                send_com_buf[15] = 0;  //VOLt
                send_com_buf[16] = 0;  //VOLT
                send_com_buf[17] = 0;  //VOLT
                send_com_buf[18] = 0;  //CHARGE
                send_com_buf[19] = 0;  //CHARGE
                send_com_buf[20] = 0;  //TEMP
                send_com_buf[21] = 0;  //TEMP
                send_com_buf[22] = 0;  //TEMP
                send_com_buf[23] = 0;  //COMM
                send_com_buf[24] = 0;  //COME_USE


            }



                 Xor = 0;
                 send_com_buf[0] = 0x16;
                 send_com_buf[1] = 0x16;
                 send_com_buf[2] = 0x16;
                 //send_com_buf[3] = 0x64;
                 send_com_buf[4] = ack_data[4];
                 send_com_buf[5] = ack_data[5];

                 send_com_buf[6] = year_int;
                 send_com_buf[7] = month_int;
                 send_com_buf[8] = day_int;
                 send_com_buf[9] = hour_int;
                 send_com_buf[10] = minute_int;
                 send_com_buf[11] = second_int;

                 for(ii =0; ii < 25 ; ii++){
                     Xor = Xor^send_com_buf[ii];
                 }
                 send_com_buf[25] = Xor; //CHKSUM
                 send_com_buf[26] = 0x0D;
                 send_com_buf[27] = 0x0A;


                     databuf = QByteArray(reinterpret_cast<char*>(send_com_buf),28);
                     otherClient->write(databuf);

                     const std::size_t count = databuf.size();
                     unsigned char* hex =new unsigned char[count];
                     std::memcpy(hex,databuf.constData(),count);
                     QString line;
                     for(int hh = 0; hh < count; hh++){
                        // qDebug() << " pData in: " << pData[hh];
                         qDebug() << " hex in: " << hex[hh];
                         line += QString::number(hex[hh]);
                         line += " ";
                     }
                     databuf.clear();


    }


        if(DATA_FLAG_NG == 1){
            DATA_FLAG_NG = 0;




            AMP = (ack_data[12]*65536 + ack_data[13]*256 + ack_data[14])/100.0;

            VOLT = (ack_data[15]*65536 + ack_data[16]*256 + ack_data[17])/100.0;

            CHARGER = (ack_data[18]*256 + ack_data[19])/10.0;

            TEMPER = (ack_data[20]*65536 + ack_data[21]*256 + ack_data[22])/100.0;

            COMM = ack_data[23];

            COMM_USE = ack_data[24];




                 Xor = 0;
                 ack_data[3] = 0x65;

                 for(ii =0; ii < 25 ; ii++){
                     Xor = Xor^ack_data[ii];
                 }
                 ack_data[25] = Xor;


                     databuf = QByteArray(reinterpret_cast<char*>(ack_data),28);
                     otherClient->write(databuf);

                     const std::size_t count = databuf.size();
                     unsigned char* hex =new unsigned char[count];
                     std::memcpy(hex,databuf.constData(),count);
                     QString line;
                     for(int hh = 0; hh < count; hh++){
                        // qDebug() << " pData in: " << pData[hh];
                         qDebug() << " hex in: " << hex[hh];
                         line += QString::number(hex[hh]);
                         line += " ";
                     }
                     databuf.clear();

    }




        if(ALARM_FLAG_NG == 1){
            ALARM_FLAG_NG = 0;

            //HANGER_NO = 4;
            //COMM = 0;
            //TIME = 0;


            HANGER_NO = ack_alarm[4] * 256 + ack_alarm[5];
            BATTERY =  ack_alarm[6];
            SELFTEST =  ack_alarm[7];

                 Xor = 0;
                 send_com_buf[0] = 0x16;
                 send_com_buf[1] = 0x16;
                 send_com_buf[2] = 0x16;
                 send_com_buf[3] = 0x65;
                 send_com_buf[4] = ack_alarm[4];
                 send_com_buf[5] = ack_alarm[5];
                 send_com_buf[6] = ack_alarm[6];
                 send_com_buf[7] = ack_alarm[7];


                 for(ii =0; ii < 8 ; ii++){
                     Xor = Xor^send_com_buf[ii];
                 }
                 send_com_buf[8] = Xor; //CHKSUM
                 send_com_buf[9] = 0x0D;
                 send_com_buf[10] = 0x0A;

                 //open_uart();

                     databuf = QByteArray(reinterpret_cast<char*>(send_com_buf),11);
                     otherClient->write(databuf);

                     const std::size_t count = databuf.size();
                     unsigned char* hex =new unsigned char[count];
                     std::memcpy(hex,databuf.constData(),count);
                     QString line;
                     for(int hh = 0; hh < count; hh++){
                        // qDebug() << " pData in: " << pData[hh];
                         qDebug() << " hex in: " << hex[hh];
                         line += QString::number(hex[hh]);
                         line += " ";
                     }
                     databuf.clear();

        }




        if(MODE_FLAG_NG == 1){
            MODE_FLAG_NG = 0;


            HANGER_NO = ack_mode[4] * 256 + ack_mode[5];
            COMM =  ack_mode[6];
            TIME = ack_mode[7] * 65536 + ack_mode[8]*256 + ack_mode[9];


                Xor = 0;
                send_com_buf[0] = 0x16;
                send_com_buf[1] = 0x16;
                send_com_buf[2] = 0x16;
                send_com_buf[3] = 0x65;
                send_com_buf[4] = ack_mode[4];
                send_com_buf[5] = ack_mode[5];
                send_com_buf[6] = ack_mode[6];
                send_com_buf[7] = ack_mode[7];
                send_com_buf[8] = ack_mode[8];
                send_com_buf[9] = ack_mode[9];

                for(ii =0; ii < 10 ; ii++){
                    Xor = Xor^send_com_buf[ii];
                }
                send_com_buf[10] = Xor; //CHKSUM
                send_com_buf[11] = 0x0D;
                send_com_buf[12] = 0x0A;

                //open_uart();

                    databuf = QByteArray(reinterpret_cast<char*>(send_com_buf),13);
                    otherClient->write(databuf);

                    const std::size_t count = databuf.size();
                    unsigned char* hex =new unsigned char[count];
                    std::memcpy(hex,databuf.constData(),count);
                    QString line;
                    for(int hh = 0; hh < count; hh++){
                       // qDebug() << " pData in: " << pData[hh];
                        qDebug() << " hex in: " << hex[hh];
                        line += QString::number(hex[hh]);
                        line += " ";
                    }
                    databuf.clear();

        }




}


