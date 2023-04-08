#include "framework.h"

void MSTPrim::FindPath(Delaunay& triangulation, const std::vector<ObLine>& linesTriangulated,
    OUT std::vector<ObLine>& linesMST)
{
    edgePq.push(linesTriangulated[0]);

    while (false == edgePq.empty())
    {
        ObLine curLine = edgePq.top();
        // 노드의 인덱스를 검사
        curLine.SetVIdx(triangulation.nodesForIndex[curLine.V()]);
        curLine.SetWIdx(triangulation.nodesForIndex[curLine.W()]);

        edgePq.pop();

        if (visited[curLine.V().index] && visited[curLine.W().index])
            continue;
        visited[curLine.V().index] = true;
        visited[curLine.W().index] = true;

        curLine.color = Color(0.5f, 1.0f, 0.5f);
        linesMST.emplace_back(curLine);

        auto Push = [&](const Vec2f& node)
        {
            size_t size = triangulation.nodesLinked[node].size();
            Vec2f nextNode;
            for (size_t i = 0; i < size; i++)
            {
                nextNode = triangulation.nodesLinked[node][i];
                nextNode.index = triangulation.nodesForIndex[nextNode];
                edgePq.push(ObLine(node, nextNode));
            }
        };
        Push(curLine.V());
        Push(curLine.W());
    }
}