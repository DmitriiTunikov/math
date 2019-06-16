#ifndef LOGGER_H
#define LOGGER_H

#include <QFile>
#include <QTextStream>

class Logger{
public:
    static void initLogger(const QString &filename);
    static void print(const QString &str);
private:
    static QFile m_file;
};


#endif // LOGGER_H
