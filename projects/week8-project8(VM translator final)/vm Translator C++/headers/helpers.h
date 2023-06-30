#include "extensions.h"
#include <sys/stat.h>
#include <filesystem>

bool isOnlyDigits(ExtendedString str)
{
    for (char i : str)
    {
        if (i < 48 || i > 57)
        {
            return false;
        }
    }
    return true;
}
