#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include <QObject>
#include <QFile>
#include <QMutex>
#include <QElapsedTimer>
#include <QDebug>
class LogManager : public QObject
{
    Q_OBJECT
    LogManager();
public:
    ~LogManager();

    static LogManager *getInstance();
    //LogManager *getInstance();
    void outputLog(QtMsgType type, const QMessageLogContext& context, const QString& msg);
    void initManager(const QString &path=QString());
    void freeManager();

signals:
    void newLog(int msgType, const QString &log);

private:
    QtMessageHandler defaultOutput = nullptr;
    QFile file;
    QString filePath;
    QMutex logMutex;
    QElapsedTimer elapsedTimer;
};

#endif // LOGMANAGER_H
