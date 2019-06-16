#include "Logger.h"

QFile Logger::m_file;

void Logger::initLogger(const QString &filename){
        if (!m_file.isOpen()){
            m_file.setFileName(filename);
            m_file.open(QIODevice::WriteOnly);
            if (!m_file.isOpen())
                qt_assert("Can't open log file", __FILE__, __LINE__);
        }
        else
            return;
    }

void Logger::print(const QString &str){
    if (m_file.isOpen()){
        QTextStream ts(&m_file);
        ts << str << "\n";
    }
}
