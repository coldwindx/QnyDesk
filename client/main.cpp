#include "MainWindow.h"
#include <QApplication>
#include <QDateTime>
#include <QFileInfo>
#include <QDir>
#include <Windows.h>
#include <QDebug>
#include "common/Logger.h"

#ifdef Q_OS_WIN
// 单一实例
bool isAlreadyRun()
{
    HANDLE hMutex = ::CreateMutexA(nullptr, FALSE, "TEST");
    return hMutex && ERROR_ALREADY_EXISTS == ::GetLastError();
}
#endif

int main(int argc, char *argv[])
{
#ifdef Q_OS_WIN
    if(isAlreadyRun()) return 0;
#endif
    GOOGLE_PROTOBUF_VERIFY_VERSION;         // 启用protobuf
    Logger::useLog(Logger::CONSOLE);
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);     // 主窗口关闭，程序不退出
    MainWindow w;
    w.show();
    int ret = a.exec();
    google::protobuf::ShutdownProtobufLibrary();    // 关闭所有protobuf的lib库
    return ret;
}

void setDebugOutput(const QString &rawTargetFilePath_, const bool &argDateFlag_)
{
    static QString rawTargetFilePath;
    static bool argDateFlag;

    rawTargetFilePath = rawTargetFilePath_;
    argDateFlag = argDateFlag_;

    class HelperClass
    {
    public:
        static void messageHandler(QtMsgType type, const QMessageLogContext &, const QString &message_)
        {
            QString message;

            switch ( type )
            {
                case QtDebugMsg:
                {
                    message = message_;
                    break;
                }
                case QtWarningMsg:
                {
                    message.append("Warning: ");
                    message.append(message_);
                    break;
                }
                case QtCriticalMsg:
                {
                    message.append("Critical: ");
                    message.append(message_);
                    break;
                }
                case QtFatalMsg:
                {
                    message.append("Fatal: ");
                    message.append(message_);
                    break;
                }
                default: { break; }
            }

            QString currentTargetFilePath;

            if ( argDateFlag )
            {
                currentTargetFilePath = rawTargetFilePath.arg( ( ( argDateFlag ) ? ( QDateTime::currentDateTime().toString("yyyy_MM_dd") ) : ( "" ) ) );
            }
            else
            {
                currentTargetFilePath = rawTargetFilePath;
            }

            if ( !QFileInfo::exists( currentTargetFilePath ) )
            {
                QDir().mkpath( QFileInfo( currentTargetFilePath ).path() );
            }

            QFile file( currentTargetFilePath );
            file.open( QIODevice::WriteOnly | QIODevice::Append );

            QTextStream textStream( &file );
            textStream << QDateTime::currentDateTime().toString( "yyyy-MM-dd hh:mm:ss" ) << ": " << message << endl;
        }
    };

    qInstallMessageHandler( HelperClass::messageHandler );
}
