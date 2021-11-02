#include <QDebug>

#include "Request.h"

static QLoggingCategory logRequest("Request", QtCriticalMsg);

static const auto idKey = QStringLiteral("id");
static const auto statusKey = QStringLiteral("status");
static const auto statusSuccess = QStringLiteral("success");
static const auto statusFailure = QStringLiteral("failure");
static const auto statusProgress = QStringLiteral("progress");

static const auto resultKey = QStringLiteral("result");

/**
    Builds a new reply from data map, result map and status

    @param request - Map with details
    @param result - Result of the handling for request
    @param status - Status of the request
*/
static QVariantMap buildReply(const QVariantMap &request,
                              const QVariant &result, const QString &status)
{
    auto reply = QVariantMap({
        {statusKey, status},
        {resultKey, result}
    });

    if (request.contains(idKey)) {
        reply[idKey] = request.value(idKey);
    }

    return reply;
}

/**
    Creates a new request with given name to a certain request class

    @param name - Name of the request
    @param parent - Pointer to the class initiating a request
*/
Request::Request(const QString &name, QObject *parent)
    : QObject(parent), mName(name)
{
}

/**
    Search for a request name in the request class.

    @param name - Request name we're searching in the class
    @return Result of searching for the request
*/
bool Request::match(const QString &name) const
{
    qCDebug(logRequest).nospace() << "Request::match(" << name << ")";
    return !mName.compare(name);
}

/**
    Emits an error signal in case of error being found.

    @param request - Request name we're emitting the error for
    @param result - Result of the handling.
*/
void Request::emitError(const QVariantMap &request, const QVariant &result)
{
    qCDebug(logRequest) << "Request" << request << "failed, result" << result;
    emit error(buildReply(request, result, statusFailure));
}

/**
    Emits a completed signal in case of a completion being done.

    @param request - Request name we're emitting the completedness for
    @param result - Result of the handling.
*/
void Request::emitCompleted(const QVariantMap &request, const QVariant &result)
{
    qCDebug(logRequest) << "Request" << request << "succeded, result" << result;
    emit completed(buildReply(request, result, statusSuccess));
}

/**
    Emits a progress signal in case of a request that takes longer than usual.

    @param request - Request name we're emitting the progress for
    @param result - Result of the handling.
*/
void Request::emitProgress(const QVariantMap &request, const QVariant &result)
{
    qCDebug(logRequest) << "Request" << request << "progress, result" << result;
    emit progress(buildReply(request, result, statusProgress));
}
