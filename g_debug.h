#ifndef G_DEBUG_H
#define G_DEBUG_H

#include <QtDebug>

#ifdef Q_OS_ANDROID
#define LOG(msg) qInfo() << msg;
#else
#define LOG(msg) qDebug() << msg;
#endif

#endif // G_DEBUG_H
