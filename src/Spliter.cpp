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
    size_t current_pos = str_.find_first_of(pattern_);

    while (true)
    {
        size_t next_pos = str_.find_first_of(pattern_, current_pos + 1);
        if (next_pos == std::string::npos)
        {
            break;
        }
        (*array).push_back(str_.substr(current_pos + 1, next_pos - current_pos - 1));
        current_pos = next_pos;
    }
}

