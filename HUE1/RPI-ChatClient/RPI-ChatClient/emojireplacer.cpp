#include "emojireplacer.h"

EmojiReplacer::EmojiReplacer() {}

void EmojiReplacer::replaceToEmojis(QString &inplaceString)
{
    for (auto it = replacements.cbegin(); it != replacements.cend(); ++it) {
        inplaceString.replace(it.key(), it.value());
    }
}
