#ifndef LOGGER_H
#define LOGGER_H

#include <QString>


class Logger
{
public:
    static void init(const QString &logFilePath, bool enableConsole = true, bool enableFile = true);

    static void shutdown();

private:
    Logger() = delete;
};

#endif // LOGGER_H
