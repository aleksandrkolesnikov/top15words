#include "WordsProvider.h"
#include <QFile>
#include <QTextStream>
#include <QThreadPool>

namespace top15words::domain
{

WordsProvider::WordsProvider(QObject* parent)
    : QObject{parent}
{
}

void WordsProvider::requestWords(const QString& filePath)
{
    token.reset(new CancelationToken{});
    auto worker = [this, filePath]() { run(filePath, token); };

    QThreadPool::globalInstance()->start(std::move(worker));
}

void WordsProvider::run(const QString& filePath, CancelationTokenPtr token)
{
    auto isCanceled = [&token]() { return token.use_count() == 1; };

    QFile file{filePath};
    if (!file.open(QIODevice::OpenModeFlag::ReadOnly))
    {
        // emit error
    }

    QTextStream stream{&file};

    while (!stream.atEnd())
    {
        std::unordered_map<QString, std::uint32_t> wordsRating;
        for (int i = 0; i < 1000; ++i)
        {
            QString word;
            stream >> word;

            if (stream.status() != QTextStream::Status::Ok)
            {
                break;
            }

            wordsRating[word] += 1;
        }

        WordSet top15Words{wordsRating.cbegin(), wordsRating.cend()};
        std::sort(top15Words.begin(), top15Words.end(),
                    [](const auto& left, const auto& right)
                    {
                        return left.second > right.second;
                    });
        top15Words.erase(top15Words.cbegin() + std::min(15ull, top15Words.size()), top15Words.cend());

        if (isCanceled())
        {
            return;
        }

        emit sendWords(top15Words);

        QThread::msleep(400);
    }
}

void WordsProvider::stop()
{
    token.reset();
}

}
