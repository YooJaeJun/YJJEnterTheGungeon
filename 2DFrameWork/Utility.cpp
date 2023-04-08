#include "framework.h"
using Microsoft::WRL::ComPtr;

Utility::RECT::RECT(Vector2 pivot, Vector2 scale)
{
    min.x = pivot.x - scale.x * 0.5f;
    min.y = pivot.y - scale.y * 0.5f;
    max.x = pivot.x + scale.x * 0.5f;
    max.y = pivot.y + scale.y * 0.5f;
    lt = { min.x, max.y };
    lb = { min.x, min.y };
    rt = { max.x, max.y };
    rb = { max.x, min.y };
}

Utility::CIRCLE::CIRCLE(const Vector2 pivot, const Vector2 scale)
{
    this->pivot = pivot;
    radius = scale.x * 0.5f;
}

Utility::LINE::LINE(const Vector2 begin, const Vector2 end)
{
    this->begin = begin;
    this->end = end;
}


int Utility::Ccw(const Vector2& v1, const Vector2& v2, const Vector2& v3)
{
    float res = v1.x * v2.y + v2.x * v3.y + v3.x * v1.y
        - (v1.y * v2.x + v2.y * v3.x + v3.y * v1.x);
    if (res > 0) return 1;
    if (res < 0) return -1;
    else return 0;
}

ColPos Utility::IntersectLineLine(const LINE& line1, const LINE& line2)
{
    //���� ��� �ݽð�, ���� ���� �ð�, ���� 0 ����	
    //(x()1, y()1) ���� (x()2, y()2) == (x()1*y()2) - (x()2*y()1).	
    //�� ������ begin���� �ٸ� ������ begin����, end���� ������ �ϳ��� ���, �ϳ��� ������ ���;� ��. �ٸ� ���� begin�� �ݺ�.

    LINE l1 = line1;
    LINE l2 = line2;

    auto compare = [&](const Vector2& l, const Vector2& r)
    {
        if (l.x == r.x) 
            return l.y <= r.y;
        return l.x <= r.x;
    };

    //�� ������ �� ���� ���� �ְų�, ������ ��ġ�� ���
    float ab = static_cast<float>(Ccw(l1.begin, l1.end, l2.begin) * Ccw(l1.begin, l1.end, l2.end));
    float cd = static_cast<float>(Ccw(l2.begin, l2.end, l1.begin) * Ccw(l2.begin, l2.end, l1.end));

    if (ab == 0.0f && cd == 0.0f)
    {
        // ���� ����
        if (compare(l1.end, l1.begin))
            swap(l1.begin, l1.end);

        if (compare(l2.end, l2.begin))
            swap(l2.begin, l2.end);

        //����
        if (compare(l2.begin, l1.end) && 
            compare(l1.begin, l2.end)) 
        {
            //���� ����
            if (l1.begin.x <= l2.begin.x && 
                l1.begin.y <= l2.begin.y && 
                l1.end.x >= l2.end.x && 
                l1.end.y >= l2.end.y) 
                return ColPos::contain;

            return ColPos::inter;
        }
    }
    //�߰����� ��ġ�� ���
    else if (ab < 0.0f && cd < 0.0f)
        return ColPos::inter;

    return ColPos::none;
}

ColPos Utility::IntersectRectCoord(const RECT& rc, const Vector2 coord)
{
    if (rc.min.x <= coord.x && coord.x <= rc.max.x &&
        rc.min.y <= coord.y && coord.y <= rc.max.y)
        return ColPos::inter;

    return ColPos::none;
}

ColPos Utility::IntersectRectLine(const RECT& rc, const LINE& l)
{
    // lt-lb  lb-rb  rb-rt  rt-lt
    LINE line1(rc.lt, rc.lb);
    LINE line2(rc.lb, rc.rb);
    LINE line3(rc.rb, rc.rt);
    LINE line4(rc.rt, rc.lt);

    if (IntersectRectCoord(rc, l.begin) &&
        IntersectRectCoord(rc, l.end))
        return ColPos::inter;

    else if (IntersectLineLine(l, line1) == ColPos::inter ||
        IntersectLineLine(l, line2) == ColPos::inter ||
        IntersectLineLine(l, line3) == ColPos::inter ||
        IntersectLineLine(l, line4) == ColPos::inter)
        return ColPos::inter;

    return ColPos::none;
}

ColPos Utility::IntersectRectRect(const RECT& rc1, const RECT& rc2)
{
    if (rc1.min.x <= rc2.max.x &&
        rc1.max.x >= rc2.min.x &&
        rc1.min.y <= rc2.max.y &&
        rc1.max.y >= rc2.min.y)
        return ColPos::inter;

    return ColPos::none;
}

ColPos Utility::IntersectRectRect(shared_ptr<GameObject> ob1, shared_ptr<GameObject> ob2)
{
    //�߽���
    Vector2 rc1Pivot = ob1->GetWorldPivot();
    Vector2 rc2Pivot = ob2->GetWorldPivot();

    //�� �߽������� ���� ����
    Vector2 dis = rc1Pivot - rc2Pivot;

    //�簢������ 2���� ����ũ���� ����
    Vector2 Rc1Up =
        ob1->GetUp() * ob1->scale.y * 0.5f;
    Vector2 Rc1Right =
        ob1->GetRight() * ob1->scale.x * 0.5f;
    
    Vector2 Rc2Up =
        ob2->GetUp() * ob2->scale.y * 0.5f;
    Vector2 Rc2Right =
        ob2->GetRight() * ob2->scale.x * 0.5f;

    //ob1�� right�� ��
    //       ���밪(���� a . b)
    float c = fabs(dis.Dot(ob1->GetRight()));

    //ob2���� �κ��Ͱ� ������ ����
    float a = fabs(Rc2Up.Dot(ob1->GetRight()))
        + fabs(Rc2Right.Dot(ob1->GetRight()));

    //ob1���� �κ��Ͱ� ������ ����
    float b = ob1->scale.x * 0.5f;

    if (c > a + b) 
        return ColPos::none;

    //ob1�� Up�� ��
    //       ���밪(���� a . b)
    c = fabs(dis.Dot(ob1->GetUp()));

    //ob2���� �κ��Ͱ� ������ ����
    a = fabs(Rc2Up.Dot(ob1->GetUp()))
        + fabs(Rc2Right.Dot(ob1->GetUp()));
    //ob1���� �κ��Ͱ� ������ ����
    b = ob1->scale.y * 0.5f;

    if (c > a + b) 
        return ColPos::none;

    //ob2�� Right�� ��
    //       ���밪(���� a . b)
    c = fabs(dis.Dot(ob2->GetRight()));

    //ob1���� �κ��Ͱ� ������ ����
    a = fabs(Rc1Up.Dot(ob2->GetRight()))
        + fabs(Rc1Right.Dot(ob2->GetRight()));
    //ob2���� �κ��Ͱ� ������ ����
    b = ob2->scale.x * 0.5f;

    if (c > a + b) 
        return ColPos::none;

    //ob2�� Up�� ��
    //       ���밪(���� a . b)
    c = fabs(dis.Dot(ob2->GetUp()));

    //ob1���� �κ��Ͱ� ������ ����
    a = fabs(Rc1Up.Dot(ob2->GetUp()))
        + fabs(Rc1Right.Dot(ob2->GetUp()));
    //ob2���� �κ��Ͱ� ������ ����
    b = ob2->scale.y * 0.5f;

    if (c > a + b) 
        return ColPos::none;

    return ColPos::inter;
}

ColPos Utility::IntersectRectCircle(const RECT& rc, const CIRCLE& cc)
{
    Vector2 rectPivot = (rc.min + rc.max) * 0.5f;
    Vector2 RectScale = rc.max - rc.min;

    RECT Wrect(rectPivot, RectScale + Vector2(cc.radius * 2.0f, 0.0f));

    if ((bool)IntersectRectCoord(Wrect, cc.pivot))
        return ColPos::leftRight;

    RECT Hrect(rectPivot, RectScale + Vector2(0.0f, cc.radius * 2.0f));
    if ((bool)IntersectRectCoord(Hrect, cc.pivot))
        return ColPos::upDown;

    Vector2 edge[4];
    edge[0] = rc.min;
    edge[1] = rc.max;
    edge[2] = Vector2(rc.min.x, rc.max.y);
    edge[3] = Vector2(rc.max.x, rc.min.y);

    for (int i = 0; i < 4; i++)
        if (IntersectCircleCoord(cc, edge[i]))
            return ColPos::edge;

    return ColPos::none;
}

/*
bool Utility::IntersectRectCircle(GameObject * ob1, GameObject * ob2, COLDIR & result)
{
    Vector2 rectPivot = ob1->GetWorldPivot();
    Vector2 rectScale = ob1->scale;

    RECT Wrect(rectPivot, rectScale + Vector2(ob2->scale.x , 0.0f));

    if (IntersectRectCoord(Wrect, ob2->GetWorldPivot()))
    {
        result = COLDIR::LEFTRIGHT;
        return true;
    }

    RECT Hrect(rectPivot, rectScale + Vector2(0.0f, ob2->scale.x));
    if (IntersectRectCoord(Hrect, ob2->GetWorldPivot()))
    {
        result = COLDIR::UPDOWN;
        return true;
    }

    Vector2 edge[4];
    RECT rc = RECT(ob1->GetWorldPivot(), ob1->scale);
    edge[0] = rc.min;
    edge[1] = rc.max;
    edge[2] = Vector2(rc.min.x, rc.max.y);
    edge[3] = Vector2(rc.max.x, rc.min.y);
    
    CIRCLE cc = CIRCLE(ob2->GetWorldPivot(), ob2->scale);
    for (int i = 0; i < 4; i++)
    {
        if (IntersectCircleCoord(cc, edge[i]))
        {
            result = COLDIR::EDGE;
            return true;
        }
    }
    result = COLDIR::NONE;
    return false;
}
*/

ColPos Utility::IntersectCircleCoord(const CIRCLE& cc, const Vector2 coord)
{
    Vector2 Distance = cc.pivot - coord;
    if (Distance.Length() < cc.radius)
        return ColPos::inter;

    return ColPos::none;
}

ColPos Utility::IntersectCircleLine(const CIRCLE& cc, const LINE& l)
{
    //���� �ڵ�� ���۸� �� ���Ǹ� �� ������.
    const Vector2& origin_to_begin = l.begin - cc.pivot;
    const Vector2& origin_to_end = l.end - cc.pivot;
    float c_begin = origin_to_begin.Dot(origin_to_begin) - cc.radius * cc.radius;
    float c_end = origin_to_end.Dot(origin_to_end) - cc.radius * cc.radius;

    //������ ���������� ���� �߽ɱ����� �Ÿ��� ���� ���������� ũ�� �ʴٸ�,
    //������ �������� ���� �ȿ� �����Ƿ� ������.
    //������ ��������, ������ �� �ȿ� �ִٸ� ����
    if (c_begin <= 0.0f && c_end <= 0.0f)
        return ColPos::contain;
    else if (c_begin <= 0.0f || c_end <= 0.0f)
        return ColPos::inter;

    Vector2 dir = l.end - l.begin;
    float length = sqrt(dir.Dot(dir));	//������
    if (length == 0.0f)
        return ColPos::none;

    const Vector2 normalized_dir = dir / length;
    float b_prime = origin_to_begin.Dot(normalized_dir);

    //������ �������� ���� �ۿ� �ְ�, ���� �߽ɿ��� ������ �������� ���ϴ� ���Ϳ� ������ ����
    //���Ͱ� �̷�� ���� 90�� �̸��̶�� �������� ����
    if (b_prime > 0.0f)
        return ColPos::none;

    //������ b' * b' - a * c�� ����ؾ� ��. �׷��� ������ ���� ���Ͱ� ���� ���͸�,
    //a�� normalized_dir.dot(normalized_dir) = 1
    //�̹Ƿ�, a�� ���� ����.
    float square_root_of_discriminant = sqrt(b_prime * b_prime - c_begin);	//discriminant == �Ǻ���

    float t1 = -b_prime + square_root_of_discriminant;
    if (t1 >= 0.0f && t1 <= length)
        return ColPos::inter;

    float t2 = -b_prime + square_root_of_discriminant;
    if (t2 >= 0.0f && t2 <= length)
        return ColPos::inter;

    return ColPos::none;
}

ColPos Utility::IntersectCircleCircle(const CIRCLE& cc1, const CIRCLE& cc2)
{
    Vector2 distance = cc1.pivot - cc2.pivot;
    if (distance.Length() < cc1.radius + cc2.radius)
        return ColPos::inter;

    return ColPos::none;
}

float Utility::DirToRadian(Vector2 Dir)
{
    //���� �������Ͱ� �ƴ϶��
    Dir.Normalize();
    return atan2f(Dir.y, Dir.x);
}