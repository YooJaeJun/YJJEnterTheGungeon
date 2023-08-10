#include "framework.h"

void Histogram::FindMaximalRectangleForStack(
    const int xStart, const int xEnd, const int yStart, const int yEnd,
    const int xSize, const int ySize, 
    OUT int& xMax, OUT int& yMax, OUT Vec2i& propSour, OUT Vec2i& propDest)
{
    // 히스토그램에서 가장 큰 직사각형 Stack 방법
    vector<int> height(xSize + 1);
    int areaMax = 0;

    for (int y = 0; y < ySize; y++)
    {
        stack<int> st;
        for (int x = 0; x <= xSize; x++)
        {
            if (x < xSize)
            {
                // 좌측하단부터 시작 기준
                const Vec2i on = Vec2i(xStart + x, yStart + y);
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

    ///
    /// 브루트 포스 - 정사각형
    ///
    //int areaMax = 0;
    //int w = 1, h = 1;

    //for (int curX = 0; curX < xSize; curX++)
    //{
    //    for (int curY = 0; curY < ySize; curY++)
    //    {
    //        bool loopBreak = false;
    //        int length = 2;

    //        while (false == loopBreak)
    //        {
    //            for (int x = 0; x < length && false == loopBreak; x++)
    //            {
    //                for (int y = 0; y < length && false == loopBreak; y++)
    //                {
    //                    const Vec2i on = Vec2i(xStart + curX + x, yStart + curY + y);

    //                    if (MAPINFO.tilemap->GetTileState(on) != TileState::floor ||
    //                        on.x < xStart || on.y < yStart ||
    //                        on.x >= xEnd || on.y >= yEnd)
    //                        loopBreak = true;
    //                }
    //            }
    //            length++;
    //            w = h = length;
    //        }

    //        if (areaMax <= w * h)
    //        {
    //            areaMax = w * h;
    //            xMax = w;
    //            yMax = h;
    //            propSour = Vec2i(xStart + curX, yStart + curY);
    //            propDest = Vec2i(xStart + curX + w - 3, yStart + curY + h - 3);
    //        }
    //    }
    //}
}