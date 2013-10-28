#include "Spliter.h"
#include <vector>
#include <string>

Spliter::Spliter(const std::string& pattern, const std::string& str)
    : pattern_(pattern), str_(str + pattern)
{
}

Spliter::~Spliter()
{
}

void Spliter::getAllFields(std::vector<std::string>* array) const
{
    (*array).clear();
    size_t currentPos = str_.find_first_of(pattern_);

    while (true)
    {
        size_t nextPos = str_.find_first_of(pattern_, currentPos + 1);
        if (nextPos == std::string::npos)
        {
            break;
        }
        (*array).push_back(str_.substr(currentPos + 1, nextPos - currentPos - 1));
        currentPos = nextPos;
    }
}

