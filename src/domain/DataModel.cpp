#include "DataModel.h"
#include <QThread>
#include <QTextStream>
#include <QFile>
#include <unordered_map>
#include <vector>
#include <QThreadPool>
#include <QUrl>
#include <QTimerEvent>

namespace top15words::domain
{

DataModel::DataModel(QObject* parent)
    : QAbstractTableModel{parent}
{
    using namespace std::chrono_literals;

    top15words.reserve(30);

    //startTimer(4s);
}

DataModel::~DataModel()
{
}

int DataModel::columnCount([[maybe_unused]] const QModelIndex& parent) const
{
    return static_cast<int>(top15words.size());
}

QVariant DataModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        return top15words[index.column()].second;
    }

    return QVariant{};
}

int DataModel::rowCount([[maybe_unused]] const QModelIndex& parent) const
{
    return 1;
}

QVariant DataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Orientation::Horizontal)
        {
            return top15words[section].first;
        }
    }

    return QVariant{};
}

void DataModel::setFile(const QString& filePath)
{
    QThreadPool::globalInstance()->start([this, filePath]() { processFile(filePath); });
}

void DataModel::processFile(const QString& filePath)
{
    QUrl url{filePath};

    QFile file{url.toLocalFile()};
    file.open(QIODevice::ReadOnly);
    QTextStream stream{&file};

    while (!stream.atEnd() /* && stream.status() == QTextStream::Ok */)
    {
        std::unordered_map<QString, std::size_t> wordsRating;
        for (int i = 0; i < 1000; ++i)
        {
            QString word;
            stream >> word;

            if (stream.status() != QTextStream::Ok)
            {
                break;
            }
            
            wordsRating[word] += 1;
        }

        std::vector<Data> newTop15{wordsRating.begin(), wordsRating.end()};
        std::sort(newTop15.begin(), newTop15.end(), [](const auto& left, const auto& right) {
                return left.second > right.second;
            });

        newTop15.erase(newTop15.cbegin() + std::min(15ull, newTop15.size()), newTop15.cend());
        updateTop15Words(std::move(newTop15));

        QThread::msleep(800);
    }
}

void DataModel::updateTop15Words(std::vector<Data>&& words)
{
    if (top15words.empty())
    {
        beginInsertColumns(QModelIndex{}, 0, static_cast<int>(words.size()));
        top15words = std::move(words);
        endResetModel();

        emit maxFreqChanged();
    }
    else
    {
        std::unordered_map<QString, std::size_t> temp{top15words.cbegin(), top15words.cend()};

        for (auto&& item : words)
        {
            temp[item.first] += item.second;
        }

        top15words.assign(temp.cbegin(), temp.cend());

        std::sort(top15words.begin(), top15words.end(),
                  [](const auto& left, const auto& right) { return left.second > right.second; });

        //beginInsertColumns(QModelIndex{}, 0, static_cast<int>(words.size()));
        top15words.erase(top15words.cbegin() + std::min(15ull, top15words.size()), top15words.cend());
        //endInsertColumns();
        emit maxFreqChanged();
        //QMetaObject::invokeMethod(this, "maxFreqChanged", Qt::QueuedConnection);

        emit dataChanged(createIndex(0, 0), createIndex(1, (int)top15words.size()), {Qt::DisplayRole});
        //const auto index1 = createIndex(0, 0);
        //const auto index2 = createIndex(1, (int)top15words.size());
        //QMetaObject::invokeMethod(this, "dataChanged", Qt::QueuedConnection, Q_ARG(QModelIndex, index1), Q_ARG(QModelIndex, index2));
    }
}

void DataModel::timerEvent(QTimerEvent* event)
{
    killTimer(event->timerId());

    setFile(QStringLiteral("C:/Users/ivche/Documents/Projects/2gis_test/sample3.txt"));
}

int DataModel::maxFreq() const
{
    return top15words.empty() ? 0 : static_cast<int>(top15words[0].second);
}

}
