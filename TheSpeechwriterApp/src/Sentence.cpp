#include "Sentence.h"

Sentence::Sentence(const QString &text, bool isFirstSentence, Statuses status)
    : m_text{text}
    , m_isFirstSentence{isFirstSentence}
    , m_status{status}
{
}

QString Sentence::getText() const
{
    return m_text;
}

void Sentence::setText(const QString &text)
{
    m_text = text;
}

bool Sentence::isFirst() const
{
    return m_isFirstSentence;
}

void Sentence::setFirst(bool isFirst)
{
    m_isFirstSentence = isFirst;
}

Sentence::Statuses Sentence::getStatus() const
{
    return m_status;
}

void Sentence::setStatus(Statuses status)
{
    m_status = status;
}
