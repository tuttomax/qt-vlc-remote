#ifndef VLCCONTROLLER_H
#define VLCCONTROLLER_H

#include <QObject>

#include "vlc.h"
#include <QSharedPointer>

class VLCController : public QObject
{
    Q_OBJECT
public:
    VLCController(QObject* parent = nullptr);

    Q_INVOKABLE void _connect(Connection* );
    Q_INVOKABLE void pause(Connection* );
    Q_INVOKABLE void resume(Connection*);
    Q_INVOKABLE void stop(Connection*);

    Q_INVOKABLE void fullscreen(Connection*);
    Q_INVOKABLE void loop(Connection*);

    Q_INVOKABLE void volume(Connection*,const QString& volume);
    Q_INVOKABLE void seek(Connection*,const QString& value);

    Q_INVOKABLE void next(Connection*);
    Q_INVOKABLE void previous(Connection*);


    Q_INVOKABLE void browse(Connection*,const QString& path);
    Q_INVOKABLE void play(Connection*,const QString& uri);

private:
    QSharedPointer<VLC> m_vlc;
};

#endif // VLCCONTROLLER_H
