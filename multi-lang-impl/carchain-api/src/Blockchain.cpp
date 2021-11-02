#include <QLoggingCategory>

#include "Blockchain.h"

static QLoggingCategory logBlockchain("Blockchain", QtCriticalMsg);

static const auto idKey = QStringLiteral("id");
static const auto resultKey = QStringLiteral("result");
static const auto statusKey = QStringLiteral("status");
static const auto failureKey = QStringLiteral("failure");
static const auto successKey = QStringLiteral("success");

/**
    Initialize the blockchain with the genesis block and difficulty 4
*/
Blockchain::Blockchain()
{
    mBlock = new Block();
    mChain.emplace_back(Block("Genesis Block"));
    mDifficulty = 4;
}

/**
    Add a new block to the chain and return the hash of it.

    @param newBlock The new block to be added to the blockchain
    @return Map wiht the details of the result along with the hash and id
    of the block that was added.
*/
QVariantMap Blockchain::addBlock(Block newBlock)
{
    newBlock.prevHash = getLastBlock().getHash();
    newBlock.mineBlock(mDifficulty);
    mChain.push_back(newBlock);

    QString hash = QString::fromStdString(newBlock.getHash());
    QVariantMap map;

    map.insert(idKey, mBlock->getLastIndex());
    map.insert(statusKey, successKey);
    map.insert(resultKey, hash);

    return map;
}

/**
    Return the last block of the chain.
    @return Last block from the blockchain.
*/
Block Blockchain::getLastBlock() const
{
    return mChain.back();
}

/**
    Adds a new block from a string data.
    @param data - String with concatenated data of the block.
    @return Map with the block details.
*/
QVariantMap Blockchain::add(const QString &data)
{
    mDataList = data.split("+");
    return addBlock(Block(data.toUtf8().constData()));
}
