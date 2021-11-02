#include "Const.h"
#include "RequestFactory.h"

namespace detail
{

/**
    Creates a new request in the factory.

    @param request - Request class we are appending to the acknowledged requests list.
*/
void RequestFactory::registerRequest(const Request *request)
{
    mRequests.append(request);
}

/**
    Creates a new request and returns a pointer to the instance.

    @param name - Name of the request
    @param parent - Pointer to the request class we've just named
*/
Request *RequestFactory::createRequest(const QString &name, QObject *parent)
{
    for (const auto &request : asConst(mRequests)) {
        if (request->match(name)) {
            const auto requestInstance = request->metaObject()->newInstance();
            Q_ASSERT(requestInstance);

            if (parent) {
                requestInstance->setParent(parent);
            }

            return qobject_cast<Request *>(requestInstance);
        }
    }

    return nullptr;
}

} // namespace detail
