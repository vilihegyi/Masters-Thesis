#include <QDebug>
#include <QMetaType>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>

#include "Car.h"

#define CAR_REQUEST_CLASS "car."

static const auto requestClass = QStringLiteral(CAR_REQUEST_CLASS);

static const auto requestAddToBlockchainKey =
        QStringLiteral(CAR_REQUEST_CLASS "addToBlockchain");

static const auto dataKey = QStringLiteral("data");
static const auto startDateKey = QStringLiteral("startDate");
static const auto endDateKey = QStringLiteral("endDate");
static const auto detailsKey = QStringLiteral("details");

/**
    Create the Car request class with initialization of the Blockchain
*/
Car::Car(QObject *parent) : Request(requestClass, parent)
{
    mBlockChain = new Blockchain();
}

/**
    Add a new block to the chain and return the hash of it.

    @param request The name of the request
    @return Result of finding or not a request within the class.
*/
bool Car::match(const QString &request) const
{
    return request.startsWith(requestClass);
}

/**
    Add a new block to the chain and return the hash of it.

    @param request The map with the request data
    @return Success or failure of the execution of the request
*/
bool Car::exec(const QVariantMap &request)
{
    Q_ASSERT(request.value("request").canConvert<QString>());
    const auto requestName = request.value("request").toString();
    Q_ASSERT(requestName.startsWith(requestClass));

    if (requests.contains(requestName)) {
        return (this->*requests[requestName])(request);
    } else {
        emitError(request,
                  QStringLiteral("Invalid request: '%1'").arg(requestName));
        return true;
    }
    return true;
}

/**
    Add a new block to the chain and return the hash of it.

    @param request The map with the request data, contains the blockchain data we'll add
    @return Always return true as adding to the blockchain shouldn't be a problem...
*/
bool Car::requestAddToBlockchain(const QVariantMap &request)
{
    // Everything's set, we can start mining! :)
    const auto data = request.value(dataKey).toMap();
    QVariantMap result = mBlockChain->add(data.value(detailsKey).toString());

    emit emitCompleted(request, result.value("result").toString());
    return true;
}

const QHash<QString, Car::RequestCallback> Car::requests = {
    {requestAddToBlockchainKey, &Car::requestAddToBlockchain}
};
