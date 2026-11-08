#ifndef EMOJIREPLACER_H
#define EMOJIREPLACER_H

#include <qmap.h>
#include <qstring.h>

class EmojiReplacer
{
public:
    EmojiReplacer();

    static void replaceToEmojis(QString &inplaceString);

private:
    inline static const QMap<QString, QString> replacements{{":)", QString::fromUtf8("🙂")},
                                              {":(", QString::fromUtf8("🙁")},
                                              {";)", QString::fromUtf8("😉")},
                                              {":D", QString::fromUtf8("😃")}};
};

#endif // EMOJIREPLACER_H
