#pragma once

#include "WordSet.h"
#include <QThread>

namespace top15words::domain
{

class WordsProvider final : public QThread
{
    Q_OBJECT

public:
    explicit WordsProvider(QObject* parent);

    void requestWords(const QString& filePath_);

signals:
    void sendWords(const WordSet& wordSet) const;

protected:
    virtual void run() override;

private:
    void stop();

private:
    QString filePath;
    bool stopCalculating;
};

}

