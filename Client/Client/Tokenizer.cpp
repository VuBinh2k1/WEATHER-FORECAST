#include "stdafx.h"
#include "Tokenizer.h"

vector<string> Tokenizer::split(const string& str, const string& separator) {
    vector<string> tmp;
    size_t beg = 0, end = string::npos;

    do {
        end = str.find(separator, beg);
        if (beg < end) {
            // std::string str.substr(position, lenght)
            // when (lenght > str.size() - position) return substr(positon, str.size() - position);
            tmp.push_back(str.substr(beg, end - beg));
        }
        beg = end + separator.size();
    } while (end != str.npos);
    
    return tmp;
}
