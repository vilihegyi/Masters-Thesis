#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <vector>
#include "Block.h"

#include <QObject>
#include <QDebug>

class Blockchain : public QObject
{
    Q_OBJECT

public:
    Blockchain();

    QVariantMap add(const QString &);

private:
    QVariantMap addBlock(Block);

    int mDifficulty;
    vector<Block> mChain;
    Block *mBlock;
    QStringList mDataList;

    Block getLastBlock() const;
};


#endif // BLOCKCHAIN_H
