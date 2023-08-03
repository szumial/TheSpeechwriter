#ifndef SENTENCE_H
#define SENTENCE_H

#include <QString>

class Sentence
{
public:
    enum class Statuses
    {
        Unique,
        Duplicate
    };

    Sentence(const QString &text = QString{}, bool isFirstSentence = false, Statuses status = Statuses::Unique);

    QString getText() const;
    void    setText(const QString &text);

    bool isFirst() const;
    void setFirst(bool isFirst);

    Statuses getStatus() const;
    void     setStatus(Statuses status);

private:
    QString  m_text;
    bool     m_isFirstSentence;
    Statuses m_status;
};

#endif // SENTENCE_H
