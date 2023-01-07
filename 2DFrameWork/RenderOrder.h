#pragma once

class RenderOrderCompare
{
public:
    bool operator()(GameObject* ob1, GameObject* ob2);
};

class RenderOrder : public Singleton<RenderOrder>
{
public:
    priority_queue<GameObject*, vector<GameObject*>, RenderOrderCompare> pq;

public:
    RenderOrder() = default;

    void push(GameObject* ob);
    void clear();
};
