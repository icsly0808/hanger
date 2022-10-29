#include "LogManager.h"

#include <QApplication>
#include <QDir>
#include <QThread>
#include <QTextStream>
#include <QDateTime>

void outputLogMessage(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    LogManager::getInstance()->outputLog(type,context,msg);
}

LogManager::LogManager()
{
    //initManager();
}

LogManager::~LogManager()
{
    freeManager();
}

LogManager *LogManager::getInstance()
{
    static LogManager instance;
    return &instance;
}

void LogManager::outputLog(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QMutexLocker locker(&logMutex);
    QString out_text;
    QTextStream stream(&out_text);
    switch (type) {
    case QtDebugMsg: stream<<"[Debug]"; break;
    case QtInfoMsg: stream<<"[Info]"; break;
    case QtWarningMsg: stream<<"[Warning]"; break;
    case QtCriticalMsg: stream<<"[Critical]"; break;
    case QtFatalMsg: stream<<"[Fatal]"; break;
    default: stream<<"[Unknown]"; break;
    }

    /*
    stream<<QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss]")
            <<msg
         <<QThread::currentThreadId();
   */

    stream<<QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss]")
            <<msg;


    if(file.isOpen()){
        if(elapsedTimer.elapsed()>1000*600){
            file.close();
            //重新计时
            elapsedTimer.restart();
        }
    }
    if(!file.isOpen()){
        file.setFileName(filePath+QString("/log_%1.txt")
                         .arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmm")));
        if(!file.open(QIODevice::WriteOnly|QIODevice::Append)){
            emit newLog(QtWarningMsg,"Open log file error:"+file.errorString()+file.fileName());
        }
    }
    if(file.isOpen()){
        stream.setDevice(&file);
        stream<<out_text<<endl;
    }

    emit newLog(type, msg);

    QString cmd_text;
    stream.setString(&cmd_text);
    switch (type) {
    case QtDebugMsg: stream<<"\033[32m"; break;
    case QtInfoMsg: stream<<"\033[34m"; break;
    case QtWarningMsg: stream<<"\033[33m"; break;
    case QtCriticalMsg: stream<<"\033[0;37;40m"; break;
    case QtFatalMsg: stream<<"\033[0;31;40m"; break;
    default: stream<<"\033[0m"; break;
    }
    stream<<out_text<<"\033[0m";
    defaultOutput(type,context,cmd_text);
}

void LogManager::initManager(const QString &path)
{
    filePath=path;
    if(filePath.isEmpty())
    {
        int argc=0;
        QApplication app(argc,nullptr);
        filePath=app.applicationDirPath()+"/log";
    }
    QDir dir(filePath);
    if(!dir.exists())
    {
        dir.mkpath(filePath);
    }
    elapsedTimer.start();
    defaultOutput=qInstallMessageHandler(outputLogMessage);
}

void LogManager::freeManager()
{
    file.close();
    qInstallMessageHandler(nullptr);
}
