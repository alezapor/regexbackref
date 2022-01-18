
#ifndef REGEX_MATCHER_MEMORYSTATE_H
#define REGEX_MATCHER_MEMORYSTATE_H


#include <vector>
#include <ostream>

/**
 * A class that represents a state with a memory list
 */
class MemoryState {
public:
    MemoryState();

    MemoryState(int num, std::string list);

    friend std::ostream &operator<<(std::ostream &os, const MemoryState &state);

    bool operator==(const MemoryState &rhs) const;

    bool operator!=(const MemoryState &rhs) const;

    bool operator<(const MemoryState &rhs) const;

    bool operator>(const MemoryState &rhs) const;

    bool operator<=(const MemoryState &rhs) const;

    bool operator>=(const MemoryState &rhs) const;

    MemoryState &operator=(int state);

    MemoryState &operator=(const MemoryState &rhs);

    bool operator>=(int state) const;

    /**
     * A function that checks if the memory list contains a variable
     * @param x a variable
     * @return the memory position that contains x (otherwise -1)
     */
    int getMemory(char x);

    int getMNum() const;

    void setMNum(int mNum);

    std::string getMMemoryList() const;

    void setMMemoryList(std::string mMemoryList);

private:
    int m_Num;
    std::string m_MemoryList;
};


#endif //REGEX_MATCHER_MEMORYSTATE_H
