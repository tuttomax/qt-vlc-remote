#ifndef VLC_H
#define VLC_H

#include <memory>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QAuthenticator>

#include "command.h"
#include "connection.h"

#ifdef Q_OS_ANDROID
namespace std {
template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args &&... args) {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
}
#endif


class VLC
{
public:
    VLC() {
        manager = std::make_unique<QNetworkAccessManager>();
    }

    void sendCommand(Connection* connection, const Command& command);

private:
    std::unique_ptr<QNetworkAccessManager> manager;
};
#endif // VLC_H
