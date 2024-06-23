#ifndef PATH_H
#define PATH_H

#include <vector>
#include <iostream>

class Path {
private:
    std::vector<int> vertices;

public:
    using iterator = std::vector<int>::iterator;
    using const_iterator = std::vector<int>::const_iterator;

    Path() {}

    void addVertex(int vertex);
    void removeLastVertex();

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;

    void printPath() const;
};

#endif // PATH_H
