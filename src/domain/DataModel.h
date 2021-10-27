#pragma once

#include "WordsProvider.h"
#include <QAbstractTableModel>

namespace top15words::domain
{

class DataModel : public QAbstractTableModel
{
    Q_OBJECT

    Q_PROPERTY(int maxFreq READ maxFreq NOTIFY maxFreqChanged)

public:
    explicit DataModel(QObject* parent = nullptr);

    virtual ~DataModel();

    virtual int columnCount(const QModelIndex& parent) const override;
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    virtual int rowCount(const QModelIndex& parent = QModelIndex{}) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

public slots:
    void setFile(const QString& filePath);
    int maxFreq() const;

private slots:
    void wordsReceived(const WordSet& wordSet);

signals:
    void maxFreqChanged() const;
    void incorrectFilePassed() const;

private:
    void updateTop15Words(const WordSet& wordSet);
    void mergeWords(const WordSet& wordSet);
    void sort();

private:
    WordsProvider wordsProvider;
    WordSet top15words;
};

}
