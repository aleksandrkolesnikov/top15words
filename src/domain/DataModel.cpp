#include "DataModel.h"
#include <QUrl>
#include <QTimerEvent>
#include <QFileInfo>
#include <algorithm>

namespace top15words::domain
{

DataModel::DataModel(QObject* parent)
    : QAbstractTableModel{parent}
    , wordsProvider{nullptr}
{
    top15words.reserve(30);

    QObject::connect(&wordsProvider, &WordsProvider::sendWords, this, &DataModel::wordsReceived,
                     Qt::ConnectionType::QueuedConnection);
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
    beginResetModel();
    top15words.clear();
    endResetModel();

    if (const QFileInfo info{filePath}; info.exists() && info.isFile())
    {
        wordsProvider.requestWords(filePath);
    }
    else
    {
        wordsProvider.stop();
        emit incorrectFilePassed();
    }
}

void DataModel::wordsReceived(const WordSet& wordSet)
{
    updateTop15Words(wordSet);
}

void DataModel::updateTop15Words(const WordSet& wordSet)
{
    if (top15words.empty())
    {
        beginInsertColumns(QModelIndex{}, 0, static_cast<int>(wordSet.size()));

        top15words = wordSet;
        sortByAlphabet();

        endInsertColumns();

        emit maxFreqChanged();
    }
    else
    {
        mergeWords(wordSet);
    }
}

void DataModel::mergeWords(const WordSet& wordSet)
{
    std::unordered_map temp{top15words.cbegin(), top15words.cend()};

    for (const auto& item : wordSet)
    {
        temp[item.first] += item.second;
    }

    top15words.assign(temp.cbegin(), temp.cend());

    std::sort(top15words.begin(), top15words.end(),
                [](const auto& left, const auto& right)
                {
                    return left.second > right.second;
                });

    top15words.erase(top15words.cbegin() + std::min(15ull, top15words.size()), top15words.cend());
    sortByAlphabet();

    emit maxFreqChanged();

    emit dataChanged(createIndex(0, 0), createIndex(0, (int)top15words.size()), {Qt::DisplayRole});
}

void DataModel::sortByAlphabet()
{
    std::sort(top15words.begin(), top15words.end(),
                [](const auto& left, const auto& right)
                {
                    return left.first < right.first;
                });
}

int DataModel::maxFreq() const
{
    if (top15words.empty())
    {
        return 0;
    }

    const auto t = std::max_element(top15words.cbegin(), top15words.cend(),
                                    [](const auto& left, const auto& right)
                                    {
                                        return left.second < right.second;
                                    });
    return t->second;
}

}
