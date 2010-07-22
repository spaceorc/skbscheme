#include "Term.h"

typedef List StringBuilder;

StringBuilder AllocateStringBuilder();
StringBuilder InternalAppendChr(StringBuilder stringBuilder, Chr chr);
LimitedStr InternalBuildString(StringBuilder stringBuilder);