#include "FileOperations.h"

#include <QFile>
#include <QTextStream>

FileOperations::FileOperations(QObject *parent)
    : QObject{parent}
{
}

QStringList FileOperations::importSentencesFromFile(const QString &filePath)
{
    QStringList importedSentences;
    QFile       file{filePath};

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return importedSentences;

    QTextStream in{&file};
    QString     text{in.readAll()};

    QStringList rawSentences{text.split('.', Qt::SkipEmptyParts)};

    for (const auto &rawSentence : rawSentences)
        if (!rawSentence.isEmpty())
            importedSentences << rawSentence;

    return importedSentences;
}

bool FileOperations::exportSentencesToFile(const QString &filePath, const QStringList &sentences)
{
    QFile file{filePath};

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream out{&file};

    for (const auto &sentence : sentences)
    {
        out << sentence;
    }

    return true;
}
