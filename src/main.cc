#include "Graph.h"
#include "AdjacencyListGraph.h"
#include <iostream>

int main() {
    Graph* graph = new AdjacencyListGraph();

    // 假设文件内容如下：
    // v 1 A start output
    // v 2 B end input
    // v 3 C start output
    // v 4 D end input
    // e 1 2 1.5 wire
    // e 2 3 2.0 combination
    // e 3 4 1.0 clkToQ
    // e 1 3 1.0 wire
    // e 3 2 1.0 combination
    graph->loadFromFile("graph.txt");

    // 打印图结构
    graph->printGraph();

    // 设置路径搜索的限制
    int maxPathsPerEnd = 2;
    int maxTotalPaths = 3;

    // 搜索路径并打印
    auto paths = graph->findAllPaths(maxPathsPerEnd, maxTotalPaths);
    std::cout << "\n=== Graph Paths ===" << std::endl;
    int pathId = 1;
    for (const auto& path : paths) {
        std::cout << "Path " << pathId++ << ": ";
        path.printPath();
    }

    delete graph;
    return 0;
}

