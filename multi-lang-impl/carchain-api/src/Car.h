#ifndef CAR_H
#define CAR_H

#include <QHash>

#include "Request.h"
#include "Singleton.h"

#include "Blockchain.h"

class Car : public Request
{
    Q_OBJECT

public:
    Q_INVOKABLE explicit Car(QObject * = nullptr);

    bool exec(const QVariantMap &) override;
    bool match(const QString &) const override;

private:
    bool requestAddToBlockchain(const QVariantMap &);

    typedef bool (Car::*RequestCallback)(const QVariantMap &);
    static const QHash<QString, RequestCallback> requests;

    Block *mBlock;
    Blockchain *mBlockChain;
};

#endif // CAR_H
