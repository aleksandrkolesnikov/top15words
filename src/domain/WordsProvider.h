#pragma once

#include "WordSet.h"
#include <QObject>
#include <memory>

namespace top15words::domain
{

class WordsProvider final : public QObject
{
    Q_OBJECT

private:
    struct CancelationToken{};

    using CancelationTokenPtr = std::shared_ptr<CancelationToken>;

public:
    explicit WordsProvider(QObject* parent);

    virtual ~WordsProvider() = default;

    void requestWords(const QString& filePath);
    void stop();

signals:
    void sendWords(const WordSet& wordSet) const;

private:
    void run(const QString& filePath, CancelationTokenPtr token);

private:
    CancelationTokenPtr token;
};

}

