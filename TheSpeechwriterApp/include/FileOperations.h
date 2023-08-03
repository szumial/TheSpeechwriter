#ifndef FILEOPERATIONS_H
#define FILEOPERATIONS_H

#include <QObject>

class FileOperations : public QObject
{
    Q_OBJECT

public:
    explicit FileOperations(QObject *parent = nullptr);

    QStringList importSentencesFromFile(const QString &filePath);
    bool        exportSentencesToFile(const QString &filePath, const QStringList &sentences);
};

#endif // FILEOPERATIONS_H
