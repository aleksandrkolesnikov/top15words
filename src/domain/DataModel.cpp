#include "DataModel.h"
#include <QUrl>
#include <QTimerEvent>

namespace top15words::domain
{

DataModel::DataModel(QObject* parent)
    : QAbstractTableModel{parent}
    , wordsProvider{nullptr}
{
    top15words.reserve(30);

    QObject::connect(&wordsProvider, &WordsProvider::sendWords, this, &DataModel::wordsReceived);

    //startTimer(4000);
}

DataModel::~DataModel()
{
    //wordsProvider.quit();
}

int DataModel::columnCount([[maybe_unused]] const QModelIndex& parent) const
{
    return top15words.size();
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

void DataModel::setFile(const QUrl& filePath)
{
    beginResetModel();
    top15words.clear();
    endResetModel();

    wordsProvider.requestWords(filePath.toLocalFile());
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
        endInsertColumns();

        emit maxFreqChanged();
    }
    else
    {
        mergeWords(wordSet);
    }


    /* std::sort(top15words.begin(), top15words.end(),
                [](const auto& left, const auto& right)
                {
                    return left.first < right.first;
                });*/
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

    // beginInsertColumns(QModelIndex{}, 0, static_cast<int>(words.size()));
    top15words.erase(top15words.cbegin() + std::min(15ull, top15words.size()), top15words.cend());
    // endInsertColumns();
    emit maxFreqChanged();
    // QMetaObject::invokeMethod(this, "maxFreqChanged", Qt::QueuedConnection);

    emit dataChanged(createIndex(0, 0), createIndex(1, (int)top15words.size()), {Qt::DisplayRole});
    // const auto index1 = createIndex(0, 0);
    // const auto index2 = createIndex(1, (int)top15words.size());
    // QMetaObject::invokeMethod(this, "dataChanged", Qt::QueuedConnection, Q_ARG(QModelIndex, index1),
    // Q_ARG(QModelIndex, index2));
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
