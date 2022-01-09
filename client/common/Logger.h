#ifndef LOGGER_H_
#define LOGGER_H_

#include <QObject>
#include <iostream>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>

class Logger : public QObject
{
    Q_OBJECT
public:
    const static int CONSOLE = 1;
    const static int FILE = 2;

    static QString pattern()
    {
        return "[%{time yyyyMMdd h:mm:ss.zzz} %{if-debug}DeBug%{endif}%{if-info}Info%{endif}%{if-warning}Warning%{endif}%{if-critical}Critical%{endif}%{if-fatal}Fatal%{endif}] %{file}:%{line} - %{message}" ;
    }
    // 设置日志输出模式
    static QtMessageHandler useLog(int mode)
    {
        qSetMessagePattern(pattern());
        if(FILE == mode)
            return qInstallMessageHandler(messageOutputFile);
        return qInstallMessageHandler(messageOutputConsole);
    }
    // 日志打印到到控制台
    static void messageOutputConsole(QtMsgType type, const QMessageLogContext & ctx, const QString & msg)
    {
        int msgLevel = 0;
        QString messgae = qFormatLogMessage(type, ctx, msg);
        if(type < msgLevel) return ;
        std::cout << messgae.toLocal8Bit().constData() << std::endl;
    }

    // 日志打印到到文件
    static void messageOutputFile(QtMsgType type, const QMessageLogContext & ctx, const QString & msg)
    {
        int msgLevel = 0;
        QString messgae = qFormatLogMessage(type, ctx, msg);
        if(type < msgLevel) return ;
        
        QFile file("logger.log");
        if(file.open(QFile::WriteOnly | QIODevice::Append))
        {
            QTextStream stream(&file);
            stream.setCodec(QTextCodec::codecForName("utf-8"));
            stream << messgae << "\r\n";
            stream.flush();
            file.close();
        }
    }

    explicit Logger(QObject * parent = nullptr) : QObject(parent) {}
    ~Logger() {}
    
};

#endif
