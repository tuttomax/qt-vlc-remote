#include "vlccontroller.h"


VLCController::VLCController(QObject *parent) : QObject(parent)
{
    m_vlc = QSharedPointer<VLC>::create();
}

void VLCController::_connect(Connection* connection)
{
    m_vlc->sendCommand(connection, Command("status.json", "", {}));
}

void VLCController::pause(Connection* connection)
{
    m_vlc->sendCommand(connection,Command("status.json","pl_forcepause",{}));
}

void VLCController::resume(Connection * connection)
{
    m_vlc->sendCommand(connection,Command("status.json","pl_forceresume",{}));
}

void VLCController::stop(Connection *connection)
{
    m_vlc->sendCommand(connection,Command("status.json","pl_stop",{}));
}

void VLCController::fullscreen(Connection * connection)
{
    m_vlc->sendCommand(connection,Command("status.json","fullscreen",{}));
}

void VLCController::loop(Connection * connection)
{
    m_vlc->sendCommand(connection,Command("status.json","pl_loop",{}));
}

void VLCController::seek(Connection *connection, const QString &value)
{
    m_vlc->sendCommand(connection,Command("status.json","seek",{Arg("val",value)}));
}

void VLCController::next(Connection *connection)
{
    m_vlc->sendCommand(connection,Command("status.json","pl_next",{}));
}

void VLCController::previous(Connection * connection)
{
    m_vlc->sendCommand(connection,Command("status.json","pl_previous",{}));
}

void VLCController::browse(Connection * connection,const QString& path)
{
    m_vlc->sendCommand(connection,Command(QString("browse.json?dir=%0").arg(path.toHtmlEscaped()),"",{}));
}

void VLCController::play(Connection * connection, const QString &uri)
{
    m_vlc->sendCommand(connection,Command("status.json","in_play",{Arg("input",uri.toHtmlEscaped())}));
}

void VLCController::volume(Connection *connection, const QString& volume)
{
    m_vlc->sendCommand(connection,Command("status.json","volume",{Arg("val",volume)}));
}

