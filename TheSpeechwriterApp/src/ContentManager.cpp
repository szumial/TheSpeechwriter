#include "ContentManager.h"

#include <QFile>
#include <QTextStream>

#include "FileOperations.h"

ContentManager::ContentManager(QObject *parent)
    : QObject{parent}
    , m_model{new SpeechwriterTableModel{this}}
{
}

SpeechwriterTableModel *ContentManager::model()
{
    return m_model;
}

int ContentManager::rowCount() const
{
    return m_model->rowCount();
}

int ContentManager::duplicatesCount() const
{
    return m_model->countDuplicateSentences();
}

int ContentManager::firstSentencesCount() const
{
    return m_model->countFirstSentences();
}

void ContentManager::addSentence()
{
    m_model->addSentence(QString{});
}

void ContentManager::removeSentence(const QModelIndexList &selectedIndexes)
{
    if (!selectedIndexes.isEmpty())
    {
        int rowToRemove{selectedIndexes.first().row()};
        m_model->removeSentence(rowToRemove);
    }
}

void ContentManager::moveSentenceUp(const QModelIndexList &selectedIndexes)
{
    if (!selectedIndexes.isEmpty())
    {
        int selectedRow{selectedIndexes.first().row()};

        if (selectedRow > 0)
        {
            m_model->moveSentenceUp(selectedRow);
            emit selectedRowChanged(selectedRow - 1);
        }
    }
}

void ContentManager::moveSentenceDown(const QModelIndexList &selectedIndexes)
{
    if (!selectedIndexes.isEmpty())
    {
        int selectedRow{selectedIndexes.first().row()};

        if (selectedRow < m_model->rowCount() - 1)
        {
            m_model->moveSentenceDown(selectedRow);
            emit selectedRowChanged(selectedRow + 1);
        }
    }
}

void ContentManager::goToNextDuplicate(const QModelIndexList &selectedIndexes)
{
    if (!selectedIndexes.isEmpty())
    {
        int currentRow{selectedIndexes.first().row()};
        QString currentText{m_model->getData(selectedIndexes.first()).toString().trimmed()};

        for (int row{currentRow + 1}; row < m_model->rowCount(); ++row)
        {
            const Sentence& sentence = m_model->getSentences().at(row);
            if (sentence.getStatus() == Sentence::Statuses::Duplicate &&
                m_model->getData(m_model->index(row, 0)).toString() == currentText.trimmed())
            {
                emit selectedRowChanged(row);
                break;
            }
        }
    }
}

void ContentManager::goToPreviousDuplicate(const QModelIndexList &selectedIndexes)
{
    if (!selectedIndexes.isEmpty())
    {
        int currentRow{selectedIndexes.first().row()};
        QString currentText{m_model->getData(selectedIndexes.first()).toString().trimmed()};

        for (int row{currentRow - 1}; row >= 0; --row)
        {
            const Sentence& sentence = m_model->getSentences().at(row);
            if (sentence.getStatus() == Sentence::Statuses::Duplicate &&
                m_model->getData(m_model->index(row, 0)).toString() == currentText.trimmed())
            {
                emit selectedRowChanged(row);
                break;
            }
        }
    }
}

void ContentManager::importSencences(const QString &filePath)
{
    if (!filePath.isEmpty())
    {
        FileOperations importHandler;
        QStringList    importedSentences{importHandler.importSentencesFromFile(filePath)};

        if (!importedSentences.isEmpty())
        {
            for (const auto &sentence : importedSentences)
            {
                if (!sentence.isEmpty() && sentence.contains(s_newlinePattern))
                    m_model->addSentence(sentence.trimmed().simplified(), true);
                else if (!sentence.trimmed().simplified().isEmpty())
                    m_model->addSentence(sentence.trimmed().simplified());
            }

            emit message(QString{"Import successful. Sencentes imported: %1"}.arg(QString::number(importedSentences.size())));
        }
        else
            emit message("Unable to import sentences.");
    }
}

void ContentManager::exportSentences(const QString &filePath)
{
    if (!filePath.isEmpty())
    {
        FileOperations  exportHandler;
        QList<Sentence> allSentences{m_model->getSentences()};
        QStringList     exportList;
        auto            it{allSentences.begin()};

        while (it != allSentences.end())
        {
            Sentence sentence{*it};
            QString  exportText{sentence.getText().trimmed().simplified()};

            if (!exportText.isEmpty())
            {
                if (!exportText.endsWith("."))
                    exportText += ".";
                exportText += " ";
            }

            auto nextIt{std::next(it)};

            if (nextIt != allSentences.end() && nextIt->isFirst())
                exportText += "\n\n";

            exportList << exportText;

            ++it;
        }

        if (exportHandler.exportSentencesToFile(filePath, exportList))
            emit message(QString{"Export successful. Sencentes exported: %1"}.arg(QString::number(exportList.size())));
        else
            emit message("Unable to export sentences.");
    }
}

QModelIndex ContentManager::getIndexAtRow(int row) const
{
    return m_model->index(row, 0);
}


