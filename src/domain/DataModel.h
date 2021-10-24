#pragma once

#include <QAbstractTableModel>
#include <QString>
#include <QTextStream>
#include <vector>

namespace top15words::domain
{

using Data = std::pair<QString, std::size_t>;

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

signals:
    void maxFreqChanged() const;

private:
    void processFile(const QString& filePath);
    void updateTop15Words(std::vector<Data>&& words);

    virtual void timerEvent(QTimerEvent* event) override;

private:
    std::vector<Data> top15words;
};

}
