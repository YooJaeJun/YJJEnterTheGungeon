#include "framework.h"


bool RenderOrderCompare::operator()(GameObject* ob1, GameObject* ob2)
{
    if ((int)ob1->zOrder == (int)ob2->zOrder)
    {
        float ob1Y = ob1->GetWorldPos().y - ob1->scale.y / 2.0f;
        float ob2Y = ob1->GetWorldPos().y - ob1->scale.y / 2.0f;
        return ob1Y < ob2Y;
    }
    return (int)ob1->zOrder > (int)ob2->zOrder;
}

void RenderOrder::push(GameObject* ob)
{
    pq.push(ob);
}

void RenderOrder::clear()
{
    while (false == pq.empty()) pq.pop();
}
