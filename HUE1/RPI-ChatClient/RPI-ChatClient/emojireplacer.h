#ifndef EMOJIREPLACER_H
#define EMOJIREPLACER_H

#include <qmap.h>
#include <qstring.h>

/** @brief Replaces supported text emoticons with Unicode emojis. */
class EmojiReplacer
{
public:
    /** @brief Creates an emoji replacer. */
    EmojiReplacer();

    /** @brief Replaces emoticons in the given text in place.
     *  @param[in,out] inplaceString Text to modify.
     */
    static void replaceToEmojis(QString &inplaceString);

private:
    inline static const QMap<QString, QString> replacements{{":)", QString::fromUtf8("🙂")},
                                              {":(", QString::fromUtf8("🙁")},
                                              {";)", QString::fromUtf8("😉")},
                                              {":D", QString::fromUtf8("😃")}};
};

#endif // EMOJIREPLACER_H
