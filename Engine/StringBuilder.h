#include "Term.h"

typedef List StringBuilder;

StringBuilder AllocateStringBuilder();
StringBuilder AppendChr(StringBuilder stringBuilder, Chr chr);
LimitedStr BuildString(StringBuilder stringBuilder);