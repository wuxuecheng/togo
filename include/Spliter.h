#ifndef SPLITER_H_
#define SPLITER_H_

#include <string>
#include <vector>

class Spliter
{
public:
    Spliter(const std::string& pattern, const std::string& str);
    ~Spliter();

    inline void reset();
    inline void moveNext();
	inline bool isEof() const;
    inline std::string getCurrentStr() const;    
    void getAllFields(std::vector<std::string>* array) const;

private:
    std::string pattern_;
    std::string str_;
    size_t currentPos_;
    size_t nextPos_;
};

void Spliter::reset()
{
    currentPos_ = str_.find_first_of(pattern_);
    nextPos_ = currentPos_ == std::string::npos ? std::string::npos : str_.find_first_of(pattern_, currentPos_ + 1);
}

void Spliter::moveNext()
{
    currentPos_ = nextPos_;
    nextPos_ = str_.find_first_of(pattern_, currentPos_ + 1);
}

bool Spliter::isEof() const
{
    return nextPos_ == std::string::npos;
}

std::string Spliter::getCurrentStr() const
{
    return str_.substr(currentPos_ + 1, nextPos_ - currentPos_ - 1);
}

#endif // SPLITER_H_

