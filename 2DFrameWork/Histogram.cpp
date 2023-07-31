#include "framework.h"

void Histogram::FindMaximalRectangleForStack(
    const int xStart, const int xEnd, const int yStart, const int yEnd,
    const int xSize, const int ySize, 
    OUT int& xMax, OUT int& yMax, OUT Vec2i& propSour, OUT Vec2i& propDest)
{
    vector<int> height(xSize + 1);
    int areaMax = 0;

    // ������׷����� ���� ū ���簢�� Stack ���
    for (int y = 0; y < ySize; y++)
    {
        stack<int> st;
        for (int x = 0; x <= xSize; x++)
        {
            if (x < xSize)
            {
                // �����ϴܺ��� ���� ����
                Vec2i on = Vec2i(xStart + x, yStart + y);
                if (MAPINFO.tilemap->GetTileState(on) == TileState::floor)
                    height[x]++;
                else
                    height[x] = 0;
            }

            // st.top() == x index
            while (!st.empty() && height[x] < height[st.top()])
            {
	            const int h = height[st.top()];
                st.pop();
	            const int w = (st.empty()) ? x : x - 1 - st.top();

                if (areaMax <= w * h)
                {
                    areaMax = w * h;
                    xMax = w;
                    yMax = h;
                    propSour = Vec2i(xStart + x - w, yStart + y - h + 1);
                    propDest = Vec2i(xStart + x - 1, yStart + y);
                }
            }
            st.push(x);
        }
    }
}