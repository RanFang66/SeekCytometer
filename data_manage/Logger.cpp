#include "Logger.h"

#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QMutex>
#include <QMutexLocker>
#include <QMessageLogContext>
#include <QDebug>

#include <iostream>

static QFile g_logFile;
static QMutex g_logMutex;
static bool g_enableConsole = true;
static bool g_enableFile = true;

static void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QMutexLocker locker(&g_logMutex);

    QString level;
    switch (type) {
    case QtDebugMsg:
        level = "DEBUG";
        break;
    case QtInfoMsg:
        level = "INFO";
        break;
    case QtWarningMsg:
        level = "WARN";
        break;
    case QtCriticalMsg:
        level = "ERROR";
        break;
    case QtFatalMsg:
        level = "FATAL";
        break;
    default:
        level = "DEBUG";
        break;
    }

    QString timeStr = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss.zzz");

    QString logLine = QString("%1 [%2] %3 (%4:%5)")
                          .arg(timeStr)
                          .arg(level)
                          .arg(msg)
                          .arg(context.file ? context.file : "")
                          .arg(context.line);

    // Write to file
    if (g_enableFile && g_logFile.isOpen()) {
        QTextStream out(&g_logFile);
        out << logLine << "\n";
        out.flush();
    }

    // Output to console
    if (g_enableConsole) {
        std::cerr << logLine.toStdString() << std::endl;
    }

    if (type == QtFatalMsg) {
        abort();
    }
}





void Logger::init(const QString &logFilePath, bool enableConsole, bool enableFile)
{
    QMutexLocker locker(&g_logMutex);

    g_enableConsole = enableConsole;
    g_enableFile = enableFile;

    if (g_enableFile) {
        g_logFile.setFileName(logFilePath);
        g_logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
    }
    qInstallMessageHandler(messageHandler);
}

void Logger::shutdown()
{
    QMutexLocker locker(&g_logMutex);

    qInstallMessageHandler(nullptr);
    if (g_logFile.isOpen()) {
        g_logFile.close();
    }
}
