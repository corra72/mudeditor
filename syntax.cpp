#include "syntax.h"

Syntax::Syntax(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;
    QTextCharFormat multiLineCommentFormat;


    multiLineCommentFormat.setForeground(Qt::darkRed);
    rule.pattern = QRegExp("\\$[c,C][0-9]{2,2}01.*");
    rule.format = multiLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp("\\$[c,C][0-9]{2,2}02.*");
    rule.format = multiLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(Qt::darkYellow);
    rule.pattern = QRegExp("\\$[c,C][0-9]{2,2}03.*");
    rule.format = multiLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(Qt::darkBlue);
    rule.pattern = QRegExp("\\$[c,C][0-9]{2,2}04.*");
    rule.format = multiLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegExp("\\$[c,C][0-9]{2,2}05.*");
    rule.format = multiLineCommentFormat;
    highlightingRules.append(rule);

	multiLineCommentFormat.setForeground(Qt::darkCyan);
    rule.pattern = QRegExp("\\$[c,C][0-9]{2,2}06.*");
    rule.format = multiLineCommentFormat;
    highlightingRules.append(rule);

	multiLineCommentFormat.setForeground(Qt::lightGray);
    rule.pattern = QRegExp("\\$[c,C][0-9]{2,2}07.*");
    rule.format = multiLineCommentFormat;
    highlightingRules.append(rule);

	multiLineCommentFormat.setForeground(Qt::darkGray);
    rule.pattern = QRegExp("\\$[c,C][0-9]{2,2}08.*");
    rule.format = multiLineCommentFormat;
    highlightingRules.append(rule);

	multiLineCommentFormat.setForeground(Qt::red);
    rule.pattern = QRegExp("\\$[c,C][0-9]{2,2}09.*");
    rule.format = multiLineCommentFormat;
    highlightingRules.append(rule);

	multiLineCommentFormat.setForeground(Qt::green);
    rule.pattern = QRegExp("\\$[c,C][0-9]{2,2}10.*");
    rule.format = multiLineCommentFormat;
    highlightingRules.append(rule);

	multiLineCommentFormat.setForeground(Qt::yellow);
    rule.pattern = QRegExp("\\$[c,C][0-9]{2,2}11.*");
    rule.format = multiLineCommentFormat;
    highlightingRules.append(rule);

	multiLineCommentFormat.setForeground(Qt::blue);
    rule.pattern = QRegExp("\\$[c,C][0-9]{2,2}12.*");
    rule.format = multiLineCommentFormat;
    highlightingRules.append(rule);

	multiLineCommentFormat.setForeground(Qt::magenta);
    rule.pattern = QRegExp("\\$[c,C][0-9]{2,2}13.*");
    rule.format = multiLineCommentFormat;
    highlightingRules.append(rule);

	multiLineCommentFormat.setForeground(Qt::cyan);
    rule.pattern = QRegExp("\\$[c,C][0-9]{2,2}14.*");
    rule.format = multiLineCommentFormat;
    highlightingRules.append(rule);

	multiLineCommentFormat.setForeground(Qt::white);
    rule.pattern = QRegExp("\\$[c,C][0-9]{2,2}15.*");
    rule.format = multiLineCommentFormat;
    highlightingRules.append(rule);


}

void Syntax::highlightBlock(const QString &text)
{

    setCurrentBlockState(0);
    QRegExp commentEndExpression("\\$[c,C][0-9]{4,4}");

    foreach (HighlightingRule rule, highlightingRules) {
        rule.pattern.setMinimal(true);
	int startIndex = 0;
	if (previousBlockState() != 1)
	   startIndex = text.indexOf(rule.pattern);

	while (startIndex >= 0) {
	   int endIndex = text.indexOf(commentEndExpression, startIndex+1);
	   int commentLength;

	   if (endIndex == -1) {
	       setCurrentBlockState(1);
	       commentLength = text.length() - startIndex;
	   } else {
	       commentLength = endIndex - startIndex + commentEndExpression.matchedLength() - 6;
               setCurrentBlockState(0);
	   }

	   setFormat(startIndex, commentLength, rule.format);
	   startIndex = text.indexOf(rule.pattern, startIndex + commentLength);
	}
   }
}
