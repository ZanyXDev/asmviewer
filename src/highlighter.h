#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>

QT_BEGIN_NAMESPACE
class QTextDocument;
QT_END_NAMESPACE

class Highlighter: public QSyntaxHighlighter
{
    Q_OBJECT

public:
    Highlighter(QTextDocument *parent = 0);

protected:
    void highlightBlock(const QString &text);

private:

    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };

    QVector<HighlightingRule> highlightingRules;
    QTextCharFormat keywordFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat registerFormat;
};

#endif // HIGHLIGHTER_H
