#include "highlighter.h"


Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;
    keywordPatterns << "\\bCLS\\b" << "\\bRET\\b" << "\\bSCD\\b"
                    << "\\bSCR\\b" << "\\bSCL\\b" << "\\b \\b"
                    << "\\bEXIT\\b"<< "\\bLOW\\b" << "\\bHIGH\\b"
                    << "\\bJP\\b"  << "\\bCALL\\b"<< "\\bSE\\b"
                    << "\\bSNE\\b" << "\\bLD\\b"  << "\\bADD\\b"
                    << "\\bOR\\b"  << "\\bXOR\\b" << "\\bSUB\\b"
                    << "\\bSHR\\b" << "\\bSUBN\\b"<< "\\bSHL\\b"
                    << "\\bSNE\\b" << "\\bRND\\b" << "\\bDRW\\b"
                    << "\\bSKP\\b" << "\\bSKNP\\b" ;

    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);

        registerFormat.setFontWeight(QFont::Light);
        registerFormat.setForeground(Qt::darkMagenta);
        rule.pattern = QRegularExpression(" (DT|ST|(\\[I\\])|F|B|HF|R|V)");
        rule.format = registerFormat;
        highlightingRules.append(rule);

        singleLineCommentFormat.setForeground(Qt::darkGreen);
        rule.pattern = QRegularExpression(";[^\n]*");
        rule.format = singleLineCommentFormat;
        highlightingRules.append(rule);

        //multiLineCommentFormat.setForeground(Qt::red);
        //quotationFormat.setForeground(Qt::darkGreen);
        //rule.pattern = QRegularExpression("\".*\"");
        //rule.format = quotationFormat;
        //highlightingRules.append(rule);

        //functionFormat.setFontItalic(true);
        //functionFormat.setForeground(Qt::blue);
        //rule.pattern = QRegularExpression("\\b[A-Za-z0-9_]+(?=\\()");
        //rule.format = functionFormat;
        //highlightingRules.append(rule);
    }
}

void Highlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
}


