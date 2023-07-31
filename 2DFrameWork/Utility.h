#pragma once

class Numeric;

namespace Utility
{
    //�浹Ÿ��
    struct RECT
    {
        Vector2 min;
        Vector2 max;
        Vector2 lt, lb, rt, rb;
        RECT(const Vector2 pivot, const Vector2 scale);
    };
    struct CIRCLE
    {
        Vector2 pivot;
        float   radius;
        CIRCLE(const Vector2 pivot, const Vector2 scale);
    };
    struct LINE
    {
        Vector2 begin;
        Vector2 end;
        LINE(Vector2 begin, Vector2 end);
    };

    int Ccw(const Vector2& v1, const Vector2& v2, const Vector2& v3);

    // �� ��
    ColPos IntersectLineLine(const LINE& line1, const LINE& line2);

    // �簢�� ��
    ColPos IntersectRectCoord(const RECT& rc, const Vector2 coord);
    // �簢�� ��
    ColPos IntersectRectLine(const RECT& rc, const LINE& l);
    // �簢�� �簢��
    ColPos IntersectRectRect(const RECT& rc1, const RECT& rc2);
    // �簢�� �簢�� OBB 
    ColPos IntersectRectRect(std::shared_ptr<GameObject> ob1, std::shared_ptr<GameObject> ob2);
    // �簢�� ��
    ColPos IntersectRectCircle(const RECT& rc, const CIRCLE& cc);

    // �� ��
    ColPos IntersectCircleCoord(const CIRCLE& cc, const Vector2 coord);
    // �� ��
    ColPos IntersectCircleLine(const CIRCLE& cc, const LINE& l);
    // �� ��
    ColPos IntersectCircleCircle(const CIRCLE& cc1, const CIRCLE& cc2);

    float DirToRadian(Vector2 Dir);

    //���ø� �Լ��� ����,���� �и� �Ұ�
    template<typename T>
    T Saturate(T src, T min = 0, T max = 1)
    {
        if (src < min)
            return min;

        if (src > max)
            return max;

        return src;
    }

    template<typename T>
    void Swap(T & a, T & b)
    {
        T temp = a;
        a = b;
        b = temp;
    }

    /*template<typename T>
    void Shuffle(T & arr, int range, int count)
    {
        for (int i = 0; i < count; i++)
        {
            int sour = rand() % range;
            int dest = rand() % range;
            Swap(arr[sour], arr[dest]);
        }
    }*/
};

