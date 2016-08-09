#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <vector>

typedef std::unordered_map<std::string, int> WordCount;

struct Greater
{
    bool operator()(const std::pair<int, WordCount::iterator>& lhs,
            const std::pair<int, WordCount::iterator>& rhs) const
    {
        return lhs.first > rhs.first;
    }
};

int main()
{
    WordCount counts;
    std::string word;
    while (std::cin >> word)
    {
        counts[word]++;
    }

    typedef std::vector<std::pair<int, WordCount::iterator> > FreqList;
    FreqList freq;
    freq.reserve(counts.size());
    for (WordCount::iterator it = counts.begin(); it != counts.end(); ++it)
    {
        freq.push_back(make_pair(it->second, it));
    }

    std::sort(freq.begin(), freq.end(), Greater());
    for (auto item : freq)
    {
        std::cout << item.first << '\t' << item.second->first << '\n';
    }

    return 0;
}
