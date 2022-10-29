#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHostInfo>
#include <qmqtt.h>
#include <QCoreApplication>
#include <QTimer>
//#include<qnetwork.h>
#include <QTime>
#include <QMessageBox>

//const QString DB_SERVER = "192.168.50.225";
//const quint16 DB_PORT = 3306;


void MainWindow::on_DB_NAME_textChanged(const QString &arg1)
{
    DB_NAME = ui->DB_NAME->text();
    db.setHostName(DB_SERVER);
    db.setPort(DB_PORT);
    db.setDatabaseName(DB_NAME);
    db.setUserName(DB_USER);

    qDebug() << "DB NAME " << DB_NAME;
}


void MainWindow::on_DB_USER_textChanged(const QString &arg1)
{
    DB_USER = ui->DB_USER->text();
    db.setHostName(DB_SERVER);
    db.setPort(DB_PORT);
    db.setDatabaseName(DB_NAME);
    db.setUserName(DB_USER);
    qDebug() << "DB USER " << DB_USER;
}


void MainWindow::on_DB_PASS_textChanged(const QString &arg1)
{
    DB_PASS = ui->DB_PASS->text();
    qDebug() << "DB PASS " << DB_PASS;
}

void MainWindow::on_DB_SERVER_textChanged(const QString &arg1)
{
   // DB_PORT = 3306;
    //temp_str = ui->DB_PORT->text();
    DB_SERVER = ui->DB_SERVER->text();
    db.setHostName(DB_SERVER);
    db.setPort(DB_PORT);
    db.setDatabaseName(DB_NAME);
    db.setUserName(DB_USER);
    qDebug() << "DB SERVER " << DB_SERVER;
}

void MainWindow::on_DB_PORT_textChanged(const QString &arg1)
{
    // DB_PORT = 3306;
     temp_str = ui->DB_PORT->text();
     DB_PORT = temp_str.toInt();
     db.setHostName(DB_SERVER);
     db.setPort(DB_PORT);
     db.setDatabaseName(DB_NAME);
     db.setUserName(DB_USER);
     qDebug() << "DB PORT " << DB_PORT;

}


void MainWindow::on_DATA_TABLE_textChanged(const QString &arg1)
{
    DATA_TABLE = arg1;
    DATA_TABLE = ui->DATA_TABLE->text();
}


void MainWindow::on_ALARM_TABLE_textChanged(const QString &arg1)
{
    ALARM_TABLE = arg1;
    ALARM_TABLE = ui->ALARM_TABLE->text();
}


void MainWindow::on_MODE_TABLE_textChanged(const QString &arg1)
{
    MODE_TABLE = arg1;
    MODE_TABLE = ui->MODE_TABLE->text();
}

void MainWindow::on_login_clicked()
{
  //trayIcon->hide();
  if (trayIcon->isVisible()) {
        trayIcon->hide();
  }

  if(DB_FLAG == 0){
    db.setHostName(DB_SERVER);
    db.setDatabaseName(DB_NAME);
    db.setUserName(DB_USER);
    db.setPassword(DB_PASS);
    db.open();
    if(db.isOpen()){
        qDebug() << "1.Database open!\n";
        DB_FLAG = 1;
        DB_SET = 1;
        QString str = "DB 서버 :";
        str += DB_SERVER;
        str += " 의 DB ";
        str += DB_NAME;
        str += " 에 접속하였습니다.";
        ui->DB_OK->setText(str);
        ui->DB_OK->setStyleSheet("QLineEdit {background-color: green;}");
        ui->login->setStyleSheet("QLineEdit {background-color: green;}");
        ui->login->setText("DB로그아웃");
        QColor col = QColor(Qt::green);
        if(col.isValid()) {
           QString qss = QString("background-color: %1").arg(col.name());
           ui->login->setStyleSheet(qss);
        }

            qDebug() << "Database open\n";
            QSqlQuery query;


            QString query_text = "select * from ";
            query_text += " log ";

            bool result = query.prepare(query_text);
            if (!result) {
                QSqlQuery qry;
                query_text = "CREATE TABLE IF NOT EXISTS ";
                query_text += " log ";
                query_text += " (id double unsigned NOT NULL AUTO_INCREMENT, ts timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP, data text, KEY `id`(`id`)) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8";
                qry.prepare( query_text );
                if( !qry.exec() )
                    qDebug() << qry.lastError();
                else
                    qDebug() << "Table1 created!";
            }

    }
    else{
        DB_FLAG = 0;
        //DB_SET = 0;
        QString str = "DB 서버에 접속을 실패하였습니다.";
        ui->DB_OK->setText(str);
        ui->DB_OK->setStyleSheet("QLineEdit {background-color: red;}");
        qDebug() << "1.Database open error!\n";
        ui->login->setStyleSheet("QLineEdit {background-color: red;}");
        ui->login->setText("DB로그인");
        QColor col = QColor(Qt::yellow);
        if(col.isValid()) {
           QString qss = QString("background-color: %1").arg(col.name());
           ui->login->setStyleSheet(qss);
        }
    }

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
  else{
      qDebug() << "1.Database close!\n";
      DB_FLAG = 0;
      DB_SET = 0;
      ui->login->setStyleSheet("QLineEdit {background-color: yellow;}");
      ui->login->setText("DB로그인");
      db.close();
      QString str = "DB";
      str += " 대기";
      ui->DB_OK->setText(str);
      ui->DB_OK->setStyleSheet("QLineEdit {background-color: yellow;}");
      QColor col = QColor(Qt::yellow);
      if(col.isValid()) {
         QString qss = QString("background-color: %1").arg(col.name());
         ui->login->setStyleSheet(qss);
      }
  }

  this->setTrayIconActions();
  this->showTrayIcon();

}

void MainWindow::on_DB_PASS_returnPressed()
{

    on_login_clicked();

}


void MainWindow::on_AUTO_DB_stateChanged(int arg1)
{
    if(ui->AUTO_DB->isChecked()){
        ui->AUTO_DB->setChecked(1);
         qDebug() <<"CHECKED";
         AUTO_DB_FLAG = 1;
    }
    else{
        ui->AUTO_DB->setChecked(0);
        qDebug() <<"NON CHECKED";
        AUTO_DB_FLAG = 0;
    }
}

void MainWindow::write_log(QString data)
{
    if(DB_FLAG == 1){
        QSqlQuery query;
        QString query_text;
        bool result;

        query_text = "INSERT INTO ";
        query_text += " log SET";
        query_text += " data = '" + data + "'";

        result = query.prepare(query_text);
         if (!result) {
             //qDebug() << "SETUP11 Query_text OK!";
            qDebug() << query.lastError();
        }

         if(!query.exec(query_text))
         {
               qDebug() << "444Can't Execute Query_text !"<< query.lastError();
         }
         else
         {
               //qDebug() << "444ETUP Query_text OK!";
         }

    }
}


void MainWindow::on_AUTO_LOG_stateChanged(int arg1)
{
}
