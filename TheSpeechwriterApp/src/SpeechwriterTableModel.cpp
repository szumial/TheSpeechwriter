#include "SpeechwriterTableModel.h"

SpeechwriterTableModel::SpeechwriterTableModel(QObject *parent)
    : QAbstractTableModel{parent}
{
    connect(this, &SpeechwriterTableModel::validate, this, &SpeechwriterTableModel::findDuplicates);
    connect(this, &SpeechwriterTableModel::validated, this, &SpeechwriterTableModel::setDuplicateStatus);
}

int SpeechwriterTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return m_sentences.size();
}

int SpeechwriterTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return static_cast<int>(Columns::Count);
}

QVariant SpeechwriterTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant{};

    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        const Sentence &sentence{m_sentences.at(index.row())};

        switch (static_cast<Columns>(index.column()))
        {
        case Columns::Order:
            return index.row() + 1;
        case Columns::Text:
            return sentence.getText();
        case Columns::Status:
            return sentence.getStatus() == Sentence::Statuses::Unique ? "Unique" : "Duplicate";
        case Columns::IsFirst:
            return sentence.isFirst();
        default:
            break;
        }
    }

    return QVariant{};
}

QVariant SpeechwriterTableModel::getData(const QModelIndex &index, Columns column) const
{
    if (!index.isValid())
        return QVariant{};

    const Sentence &sentence{m_sentences.at(index.row())};

    switch (column)
    {
    case Columns::Order:
        return index.row() + 1;
    case Columns::Text:
        return sentence.getText();
    case Columns::Status:
        return sentence.getStatus() == Sentence::Statuses::Unique ? "Unique" : "Duplicate";
    case Columns::IsFirst:
        return sentence.isFirst();
    default:
        break;
    }

    return QVariant{};
}

bool SpeechwriterTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole)
    {
        int row{index.row()};
        int column{index.column()};

        if (column == static_cast<int>(Columns::Text))
        {
            if (row < rowCount())
            {
                QString newText{value.toString().trimmed()};
                QString oldText{m_sentences[row].getText().trimmed()};

                if (newText != oldText)
                {
                    m_sentences[row].setText(newText);
                    emit validate(newText);
                    emit dataChanged(index, index);
                }
            }
            else
            {
                beginInsertRows(QModelIndex(), row, row);
                m_sentences.append({value.toString(), false});
                endInsertRows();
            }

            return true;
        }
        else if (column == static_cast<int>(Columns::IsFirst))
        {
            m_sentences[row].setFirst(value.toBool());
            emit dataChanged(index, index);
            return true;
        }
    }

    return false;
}

QVariant SpeechwriterTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        auto it{m_tableHeaders.find(static_cast<Columns>(section))};
        if (it != m_tableHeaders.end())
            return *it;
    }

    return QVariant();
}

Qt::ItemFlags SpeechwriterTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    auto column{index.column()};

    if (column == static_cast<int>(Columns::Text) || column == static_cast<int>(Columns::IsFirst))
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;

    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

void SpeechwriterTableModel::addSentence(const QString &text, bool isFirst)
{
        beginInsertRows(QModelIndex{}, rowCount(), rowCount());
        m_sentences.append({text, isFirst});
        endInsertRows();

        emit validate(text);
}

void SpeechwriterTableModel::removeSentence(int row)
{
    if (row < 0 || row >= rowCount())
        return;

    QString removedSentenceText{m_sentences.at(row).getText()};

    beginRemoveRows(QModelIndex{}, row, row);
    m_sentences.removeAt(row);
    endRemoveRows();

    emit validate(removedSentenceText);
}

const QList<Sentence> &SpeechwriterTableModel::getSentences() const
{
    return m_sentences;
}

void SpeechwriterTableModel::moveSentenceUp(int row)
{
    if (row > 0 && row < rowCount())
    {
        beginMoveRows(QModelIndex{}, row, row, QModelIndex{}, row - 1);
        m_sentences.move(row, row - 1);
        endMoveRows();
    }
}

void SpeechwriterTableModel::moveSentenceDown(int row)
{
    if (row >= 0 && row < rowCount() - 1)
    {
        beginMoveRows(QModelIndex{}, row, row, QModelIndex{}, row + 2);
        m_sentences.move(row, row +1);
        endMoveRows();
    }
}

int SpeechwriterTableModel::countDuplicateSentences() const
{
    int count{0};

    for (const auto &sentence : m_sentences)
        if (sentence.getStatus() == Sentence::Statuses::Duplicate)
            count++;

    return count;
}

int SpeechwriterTableModel::countFirstSentences() const
{
    int count{0};

    for (const auto &sentence : m_sentences)
        if (sentence.isFirst())
            count++;

    return count;
}

void SpeechwriterTableModel::findDuplicates(const QString &searchText)
{
    QList<int> duplicateRows;
    QString    searchTextSanitized{searchText.trimmed().simplified()};

    for (int row{0}; row < rowCount(); ++row)
    {
        const Sentence &sentence{m_sentences.at(row)};
        QString sentenceSanitized{sentence.getText().trimmed().simplified()};

        if (sentenceSanitized == searchTextSanitized)
            duplicateRows.append(row);
    }

    emit validated(duplicateRows);
}

void SpeechwriterTableModel::setDuplicateStatus(const QList<int> &duplicateRows)
{
    if (duplicateRows.isEmpty())
        return;

    if (duplicateRows.size() == 1)
    {
        m_sentences[duplicateRows.first()].setStatus(Sentence::Statuses::Unique);
        QModelIndex index{this->index(duplicateRows.first(), static_cast<int>(Columns::Status))};
        emit dataChanged(index, index);
    }
    else
    {
        for (auto duplicateRow : duplicateRows)
        {
            if (m_sentences[duplicateRow].getStatus() != Sentence::Statuses::Duplicate)
            {
                m_sentences[duplicateRow].setStatus(Sentence::Statuses::Duplicate);
                QModelIndex index{this->index(duplicateRow, static_cast<int>(Columns::Status))};
                emit dataChanged(index, index);
            }
        }
    }
}
