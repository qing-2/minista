#include "AdjacencyListGraph.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stack>
#include <algorithm>
#include <iomanip>

void AdjacencyListGraph::findAllPathsUtil(int u, int d, std::unordered_set<int>& visited, Path& path, std::vector<Path>& paths, int maxPathsPerEnd, int& totalPaths, int maxTotalPaths) const {
    // 剪枝条件：如果路径数超过限制，或者总路径数超过限制，则终止搜索
    if (paths.size() >= static_cast<size_t>(maxPathsPerEnd) || totalPaths >= maxTotalPaths) return;

    visited.insert(u);
    path.addVertex(u);

    // 如果当前顶点是目标顶点，则保存路径
    if (u == d) {
        paths.push_back(path);
        ++totalPaths;
    } else {
        // 遍历当前顶点的所有邻居顶点
        auto it = adjacencyList.find(u);
        if (it != adjacencyList.end()) {
            for (const auto& edge : it->second) {
                int neighbor = edge.getTo();
                if (!visited.count(neighbor)) {
                    findAllPathsUtil(neighbor, d, visited, path, paths, maxPathsPerEnd, totalPaths, maxTotalPaths);
                }
            }
        }
    }

    // 回溯到上一个顶点
    path.removeLastVertex();
    visited.erase(u);
}

void AdjacencyListGraph::addVertex(const Vertex& vertex) {
    vertices[vertex.getId()] = vertex;
}

void AdjacencyListGraph::addEdge(const Edge& edge) {
    adjacencyList[edge.getFrom()].push_back(edge);
}

void AdjacencyListGraph::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Could not open the file!" << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;
        if (type == "v") {
            int id;
            std::string data;
            std::string vertexTypeStr, vertexDirStr;
            iss >> id >> data >> vertexTypeStr >> vertexDirStr;
            VertexType vertexType = (vertexTypeStr == "start") ? VertexType::START : VertexType::END;
            VertexDir vertexDir = (vertexDirStr == "input") ? VertexDir::INPUT : VertexDir::OUTPUT;
            NodeInfo* info = new NodeInfo(data);
            addVertex(Vertex(id, info, vertexType, vertexDir));
        } else if (type == "e") {
            int from, to;
            double weight;
            std::string edgeTypeStr;
            iss >> from >> to >> weight >> edgeTypeStr;
            EdgeType edgeType;
            if (edgeTypeStr == "wire") {
                edgeType = EdgeType::WIRE;
            } else if (edgeTypeStr == "combination") {
                edgeType = EdgeType::COMBINATION;
            } else if (edgeTypeStr == "clkToQ") {
                edgeType = EdgeType::CLK_TO_Q;
            } else {
                throw std::invalid_argument("Unknown edge type: " + edgeTypeStr);
            }
            addEdge(Edge(from, to, weight, edgeType));
        }
    }
    file.close();
}

void AdjacencyListGraph::printGraph() const {
    std::cout << "=== Graph Structure ===" << std::endl;
    std::cout << std::left;

    std::cout << std::setw(10) << "Vertex ID" 
              << std::setw(20) << "Data" 
              << std::setw(10) << "Type" 
              << std::setw(10) << "Direction" 
              << std::endl;
    std::cout << "------------------------------------------------------------" << std::endl;

    for (const auto& [id, vertex] : vertices) {
        std::cout << std::setw(10) << id 
                  << std::setw(20) << vertex.getInfo()->data 
                  << std::setw(10) << (vertex.getType() == VertexType::START ? "start" : "end")
                  << std::setw(10) << (vertex.getDir() == VertexDir::INPUT ? "input" : "output") 
                  << std::endl;
    }

    std::cout << "\n=== Edges ===" << std::endl;
    std::cout << std::setw(10) << "From" 
              << std::setw(10) << "To" 
              << std::setw(15) << "Weight" 
              << std::setw(15) << "Type" 
              << std::endl;
    std::cout << "------------------------------------------------------------" << std::endl;

    for (const auto& [from, edges] : adjacencyList) {
        for (const auto& edge : edges) {
            std::cout << std::setw(10) << edge.getFrom() 
                      << std::setw(10) << edge.getTo() 
                      << std::setw(15) << edge.getWeight()
                      << std::setw(15) << (edge.getType() == EdgeType::WIRE ? "wire" 
                        : (edge.getType() == EdgeType::COMBINATION ? "combination" 
                        : "clkToQ")) 
                      << std::endl;
        }
    }
}


bool AdjacencyListGraph::breakCycleUtil(int v, std::unordered_set<int>& visited, std::unordered_set<int>& recStack) {
    if (!visited.count(v)) {
        visited.insert(v);
        recStack.insert(v);

        for (const auto& edge : adjacencyList[v]) {
            int neighbor = edge.getTo();
            if (!visited.count(neighbor) && breakCycleUtil(neighbor, visited, recStack))
                return true;
            else if (recStack.count(neighbor)) {
                // Break the cycle by removing the edge
                adjacencyList[v].erase(std::remove_if(adjacencyList[v].begin(), adjacencyList[v].end(),
                                                      [neighbor](const Edge& e) { return e.getTo() == neighbor; }),
                                       adjacencyList[v].end());
                return true;
            }
        }
    }
    recStack.erase(v);
    return false;
}

void AdjacencyListGraph::breakLoop() {
    std::unordered_set<int> visited;
    std::unordered_set<int> recStack;

    for (const auto& [id, _] : vertices) {
        if (!visited.count(id)) {
            breakCycleUtil(id, visited, recStack);
        }
    }
}

std::vector<Path> AdjacencyListGraph::findAllPaths(int maxPathsPerEnd, int maxTotalPaths) const {
    std::vector<Path> paths;
    std::unordered_set<int> visited;
    Path path;
    int totalPaths = 0;

    for (const auto& [startId, startVertex] : vertices) {
        if (startVertex.getType() == VertexType::START) {
            for (const auto& [endId, endVertex] : vertices) {
                if (endVertex.getType() == VertexType::END) {
                    std::vector<Path> localPaths;
                    findAllPathsUtil(startId, endId, visited, path, localPaths, maxPathsPerEnd, totalPaths, maxTotalPaths);
                    paths.insert(paths.end(), localPaths.begin(), localPaths.end());
                    if (totalPaths >= maxTotalPaths) {
                        break;
                    }
                }
            }
        }
    }
    return paths;
}

AdjacencyListGraph::~AdjacencyListGraph() {
    for (auto& [id, vertex] : vertices) {
        delete vertex.getInfo();
    }
}
