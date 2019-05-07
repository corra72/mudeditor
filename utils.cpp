#include "utils.h"
#include <QFile>
#include <QDir>
#include <ctype.h>
#include "bit.h"
#include "errno.h"

namespace ts
{
    QString Utils::m_entityReading = "";

    QString Utils::readString(FILE* pFile, const QString &defString)
    {
        char buf[ MaxStringLength ];
        int i = 0, tmp;

        buf[ 0 ] = '\0';

        while (i < MaxStringLength - 3) {
            if ((tmp = fgetc(pFile)) == EOF) {
                qWarning("Error '%s' %s in Utils::readString.", strerror(errno), m_entityReading.toUtf8().data());
                return defString;
            }

            if (tmp == '~') {
                break;
            }

            buf[ i++ ] = (char)tmp;
        }

        if (i >= MaxStringLength - 3) {
            /* We filled the buffer */
            qWarning("File too long %s in Utils::readString.", m_entityReading.toUtf8().data());
            while ((tmp = fgetc(pFile)) != EOF)
                if (tmp == '~')
                    break;
        }

        buf[ i ] = '\0';

        // Salta tutti i caratteri fino al newline (compreso) dopo il ~
        bool bNewLineFound = false;
        while ((tmp = fgetc(pFile)) != EOF) {
            if (tmp == '\n' || tmp == '\r')
                bNewLineFound = true;
            else if (bNewLineFound)
                break;
        }

        if (tmp != EOF)
            ungetc(tmp, pFile);

        return QString(buf);
    }

    void Utils::writeNewFlags(QTextStream& stream, BitVector Flags, BitVector MoreFlags)
    {
        short c = -1;

        if (MoreFlags)
            c = 0;
        if (c == -1) {/* Scrittura vecchio stile */
            saveBitVector(stream, Flags);
            stream << " " << flush;
        } else {
            stream << "-1 " << c + 2 << " " << flush;
            saveBitVector(stream, Flags);
            stream << " " << flush;
            saveBitVector(stream, MoreFlags);
            stream << " " << flush;
        }
    }

    long Utils::readNewFlags(FILE *pFile, BitVector *Flags)
    {
        long n = readNumber(pFile, QString("")), v;

        if (n != -1L) /* Flag vecchio stile */
            v = n;
        else {
            n = readNumber(pFile, QString("")); /* numero di flag presenti */
            v = readNumber(pFile, QString("")); /* primo flag */
            *Flags = readNumber(pFile, QString(""));
        }
        return(v);
    }

    long Utils::readNumber(FILE* pFile, const QString& error, long defNumber)
    {
        long number;
        bool sign;
        char c;

        do {
            c = getc(pFile);
        } while (isspace(c));

        number = 0;
        sign = false;

        if (c == '+') {
            c = getc(pFile);
        } else if (c == '-') {
            sign = true;
            c = getc(pFile);
        }

        if (!isdigit(c)) {
            fprintf(stderr, "%s: invalid number read in char '%c'.\n\r", error.toUtf8().data(), c);
            ungetc(c, pFile);
            return defNumber;
        }

        while (isdigit(c)) {
            number = number * 10 + c - '0';
            c = getc(pFile);
        }

        if (sign)
            number = 0 - number;

        if (c == '|')
            number += readNumber(pFile, error, 0);
        else if (c != ' ')
            ungetc(c, pFile);

        return number;
    }

    long Utils::readNumberInLine(FILE* pFile, const QString& error, long defNumber)
    {
        long number;
        bool sign;
        char c;

        do {
            c = getc(pFile);
            if (c == '\n')
                return -1L;
        } while (isspace(c));

        number = 0;
        sign = false;

        if (c == '+') {
            c = getc(pFile);
        } else if (c == '-') {
            sign = true;
            c = getc(pFile);
        }

        if (!isdigit(c)) {
            fprintf(stderr, "%s: invalid number read in char '%c'.\n\r", error.toUtf8().data(), c);
            ungetc(c, pFile);
            return defNumber;
        }

        while (isdigit(c)) {
            number = number * 10 + c - '0';
            c = getc(pFile);
        }

        if (sign)
            number = 0 - number;

        if (c == '|')
            number += readNumber(pFile, error, 0);
        else if (c != ' ')
            ungetc(c, pFile);

        return number;
    }

    Dice Utils::readDice(FILE* pFile)
    {
        int iDices = 0;
        int iFaces = 0;
        int iPlus = 0;
        if (fscanf(pFile, " %dd%d+%d ", &iDices, &iFaces, &iPlus) == 3) {
            Dice new_dice;
            new_dice.setNum(iDices);
            new_dice.setFaces(iFaces);
            new_dice.setBonus(iPlus);
            return new_dice;
        } else {
            qWarning("Invalid dice string read.");
            return Dice();
        }
    }

    void Utils::saveBitVector(QTextStream& stream, BitVector bitvector)
    {
        QString sSave = "";

        if (bitvector > 0) {
            for (int i = 0; i < MaxBitFlag; i++) {
                if (Bit::has(bitvector, i)) {
                    if (!sSave.isEmpty())
                        sSave += QString("|");
                    sSave += QString::number(Bit::flagToBit(i));
                }
            }
        } else {
            sSave = "0";
        }

        stream << sSave;
    }

    QString Utils::stripSpaceAtEnd(const QString& txt)
    {
        int iLen = txt.length() - 1;
        if (iLen <= 0)
            return "";

        QString sRet = txt;
        QChar c = sRet.at(iLen);
        while (c.isSpace()) {
            sRet.remove(iLen, 1);
            iLen--;
            if (iLen >= 0)
                c = sRet.at(iLen);
            else
                c = 'k';
        }

        return sRet;
    }

    QString Utils::createPath(const QString& value_dir, const QString& value_file, const QString& value_suffix)
    {
        QString sRet = "";

        if (value_dir.isEmpty())
            return sRet;

        sRet = value_dir;

        if (!value_file.isEmpty()) {
            if (!sRet.endsWith('/') && !sRet.endsWith('\\'))
                sRet += '/';

            sRet += value_file;

            if (!value_suffix.isEmpty()) {
                sRet += '.';
                sRet += value_suffix;
            }
        }

        // return QDir::convertSeparators(sRet);
        return QDir::toNativeSeparators(sRet);
    }

    QString Utils::vnumber2string(VNumber vnum, int num_chars)
    {
        QString sRet = QString::number(vnum);
        return sRet.rightJustified(num_chars, '0');
    }

    VNumber Utils::string2vnumber(const QString& txt)
    {
        if (txt.isNull() || txt.isEmpty())
            return VNumberInvalid;

        QString sTmp = txt;
        while (sTmp.startsWith('0'))
            sTmp.remove(0, 1);

        if (sTmp.isNull() || sTmp.isEmpty())
            return 0;

        return sTmp.toULong();
    }

    int Utils::double2int(double num)
    {
        int iRet = static_cast< int >(num);
        if ((num - iRet) >= 0.5)
            iRet++;
        return iRet;
    }

    QString Utils::bitvector2string(BitVector flags, const QStringList &list, const QString& separator)
    {
        QString sRet = "";
        bool first_elem = true;
        for (int i = 0; i < list.size(); i++) {
            if (Bit::has(flags, i)) {
                if (!first_elem)
                    sRet += separator;

                sRet += list.at(i);
                first_elem = false;
            }
        }

        if (sRet.isEmpty())
            sRet = "EMPTY";

        return sRet;
    }

    QString Utils::removeTilde(const QString& txt)
    {
        if (txt.isNull() || txt.trimmed().isEmpty())
            return QString("");

        QString sRet = txt;
        sRet.remove('~');
        return sRet;
    }

    QString Utils::object2string(VNumber vnum, const QString& txt)
    {
        QString sRet = Utils::vnumber2string(vnum);

        if (!txt.isNull() && !txt.isEmpty()) {
            sRet += " - ";
            sRet += txt;
        }

        return sRet;
    }

} // namespace ts
