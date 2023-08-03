#ifndef SPEECHWRITERTABLEMODEL_H
#define SPEECHWRITERTABLEMODEL_H

#include <QAbstractTableModel>
#include <QObject>

#include "Sentence.h"

class SpeechwriterTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum class Columns
    {
        Order,
        Text,
        Status,
        IsFirst,
        Count
    };

    explicit SpeechwriterTableModel(QObject *parent = nullptr);

    int           rowCount(const QModelIndex& parent = QModelIndex{}) const override;
    int           columnCount(const QModelIndex& parent = QModelIndex{}) const override;
    QVariant      data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant      getData(const QModelIndex& index, Columns column = Columns::Text) const;
    bool          setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
    QVariant      headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

    void addSentence(const QString &text = nullptr, bool isFirst = false);
    void removeSentence(int row);

    const QList<Sentence> &getSentences() const;

    void moveSentenceUp(int row);
    void moveSentenceDown(int row);

    int countDuplicateSentences() const;
    int countFirstSentences() const;

signals:
    void validate(const QString &text);
    void validated(const QList<int> &rows);

private:
    void findDuplicates(const QString &searchText);
    void setDuplicateStatus(const QList<int> &duplicateRows);

    QList<Sentence>           m_sentences;
    QMap<Columns, QByteArray> m_tableHeaders{{Columns::Order, "#"},
                                             {Columns::Text, "Text"},
                                             {Columns::Status, "Status"},
                                             {Columns::IsFirst, "Is First Sentence"}};
};

#endif // SPEECHWRITERTABLEMODEL_H
