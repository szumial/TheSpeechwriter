#ifndef CONTENTMANAGER_H
#define CONTENTMANAGER_H

#include <QObject>
#include <QRegularExpression>

#include "SpeechwriterTableModel.h"

class ContentManager : public QObject
{
    Q_OBJECT

public:
    explicit ContentManager(QObject *parent = nullptr);

    SpeechwriterTableModel *model();

    int rowCount() const;
    int duplicatesCount() const;
    int firstSentencesCount() const;

    void addSentence();
    void removeSentence(const QModelIndexList &selectedIndexes);

    void moveSentenceUp(const QModelIndexList &selectedIndexes);
    void moveSentenceDown(const QModelIndexList &selectedIndexes);

    void goToNextDuplicate(const QModelIndexList &selectedIndexes);
    void goToPreviousDuplicate(const QModelIndexList &selectedIndexes);

    void importSencences(const QString &filePath);
    void exportSentences(const QString &filePath);

    QModelIndex getIndexAtRow(int row) const;

signals:
    void selectedRowChanged(int row);
    void message(const QString &message);

private:
    SpeechwriterTableModel *m_model;

    static inline const QRegularExpression s_newlinePattern{"\\n+"};
};

#endif // CONTENTMANAGER_H
