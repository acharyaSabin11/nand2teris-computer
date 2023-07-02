#include "extensions.h"
#include <sys/stat.h>
#include <filesystem>

// checks if the provided string contains only digits or not.
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
