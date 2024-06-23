#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include "Path.h"

enum class VertexType {
    START,
    END
};

enum class EdgeType {
    WIRE,
    COMBINATION,
    CLK_TO_Q
};

enum class VertexDir {
    INPUT,
    OUTPUT
};

class NodeInfo {
public:
    std::string data;
    NodeInfo(const std::string& data) : data(data) {}
};

class Vertex {
private:
    int id;
    NodeInfo* info;
    VertexType type;
    VertexDir dir;

public:
    Vertex() : id(0), info(nullptr), type(VertexType::START), dir(VertexDir::INPUT) {}
    Vertex(int id, NodeInfo* info, VertexType type, VertexDir dir) : id(id), info(info), type(type), dir(dir) {}

    int getId() const {
        return id;
    }

    void setId(int id) {
        this->id = id;
    }

    NodeInfo* getInfo() const {
        return info;
    }

    void setInfo(NodeInfo* info) {
        this->info = info;
    }

    VertexType getType() const {
        return type;
    }

    void setType(VertexType type) {
        this->type = type;
    }

    VertexDir getDir() const {
        return dir;
    }

    void setDir(VertexDir dir) {
        this->dir = dir;
    }
};

class Edge {
private:
    int from;
    int to;
    double weight;
    EdgeType type;

public:
    Edge(int from, int to, double weight, EdgeType type) : from(from), to(to), weight(weight), type(type) {}

    int getFrom() const {
        return from;
    }

    void setFrom(int from) {
        this->from = from;
    }

    int getTo() const {
        return to;
    }

    void setTo(int to) {
        this->to = to;
    }

    double getWeight() const {
        return weight;
    }

    void setWeight(double weight) {
        this->weight = weight;
    }

    EdgeType getType() const {
        return type;
    }

    void setType(EdgeType type) {
        this->type = type;
    }
};

class Graph {
public:
    virtual ~Graph() {}
    virtual void addVertex(const Vertex& vertex) = 0;
    virtual void addEdge(const Edge& edge) = 0;
    virtual void loadFromFile(const std::string& filename) = 0;
    virtual void printGraph() const = 0;
    virtual void breakLoop() = 0;
    virtual std::vector<Path> findAllPaths(int maxPathsPerEnd, int maxTotalPaths) const = 0;
};

#endif // GRAPH_H
