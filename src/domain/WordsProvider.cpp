#include "WordsProvider.h"
#include <QFile>
#include <QTextStream>

namespace top15words::domain
{

WordsProvider::WordsProvider(QObject* parent)
    : QThread{parent}
    , stopCalculating{false}
{
}

void WordsProvider::requestWords(const QString& filePath_)
{
    filePath = filePath_;

    if (isRunning())
    {
        stop();
    }

    stopCalculating = false;
    start();
}

void WordsProvider::run()
{
    QFile file{filePath};
    if (!file.open(QIODevice::OpenModeFlag::ReadOnly))
    {
        // emit error
    }

    QTextStream stream{&file};

    while (!stream.atEnd() && !stopCalculating)
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

        emit sendWords(top15Words);

        QThread::msleep(500);
    }
}

void WordsProvider::stop()
{
    stopCalculating = true;
    wait();
}

}
