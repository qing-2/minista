#include "Path.h"

void Path::addVertex(int vertex) {
    vertices.push_back(vertex);
}

void Path::removeLastVertex() {
    vertices.pop_back();
}

Path::iterator Path::begin() {
    return vertices.begin();
}

Path::iterator Path::end() {
    return vertices.end();
}

Path::const_iterator Path::begin() const {
    return vertices.begin();
}

Path::const_iterator Path::end() const {
    return vertices.end();
}

void Path::printPath() const {
    for (int vertex : vertices) {
        std::cout << vertex << " ";
    }
    std::cout << std::endl;
}
