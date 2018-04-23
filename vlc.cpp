#include "vlc.h"
#include <memory.h>
#include <QJsonDocument>
#include <QUrlQuery>
#include "g_debug.h"

void VLC::sendCommand(Connection *connection, const Command &command) {
  Q_ASSERT(!command.page.isEmpty());

  QUrl baseUrl = QUrl::fromUserInput(QString("http://%0:%1/requests/%2")
                                         .arg(connection->ip())
                                         .arg(connection->port())
                                         .arg(command.page));
  QUrlQuery query;
  if (!command.command.isEmpty()) {
    query.addQueryItem("command", command.command);
    if (command.args.length() > 0) {
      for (const Arg &argument : command.args) {
        query.addQueryItem(argument.name, argument.value);
      }
    }
    baseUrl.setQuery(query);
  }
  Q_ASSERT(baseUrl.isValid());

  LOG(baseUrl.toString());

  QNetworkRequest request(baseUrl);
  request.setRawHeader(
      "Authorization",
      "Basic " +
          QByteArray(
              QString(":%1").arg(connection->password()).toUtf8().toBase64()));

  QNetworkReply *reply = manager->get(request);
  QObject::connect(reply, &QNetworkReply::finished, [=]() {

    if (reply->error() == QNetworkReply::NoError) {
      connection->setStatus(ConnectionStatus::Connected);
      auto response = reply->readAll();
      QJsonParseError error;
      auto doc = QJsonDocument::fromJson(response,&error);
      LOG(error.errorString());
      QJsonObject root = doc.object();
      if (root.contains("element")) connection->setInodes(root.take("element").toArray());
      else connection->setContext(root);

    } else {
      qDebug() << reply->errorString();
      connection->setStatus(ConnectionStatus::NoConnectionOrError);
      connection->setContext(QJsonObject());
      connection->setError(reply->errorString());
    }
  });
}
