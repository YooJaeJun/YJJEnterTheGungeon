#pragma once
// »ï°¢Çü
class ObTriangle : public GameObject, public StaticVertexCount<ObTriangle>
{
private:
    static ID3D11Buffer* fillVertexBuffer;
    static ID3D11Buffer* vertexBuffer;

public:
    static void CreateStaticMember();
    static void DeleteStaticMember();
    void Render();


public:
    ObNode a;
    ObNode b;
    ObNode c;

public:
    bool isBad = false;

public:
    ObTriangle();
    ObTriangle(const ObTriangle&) = default;
    ObTriangle(ObTriangle&&) noexcept = default;
    ObTriangle(const Vector2& v1, const Vector2& v2, const Vector2& v3);
    ObTriangle(const ObNode& v1, const ObNode& v2, const ObNode& v3);

    ObTriangle& operator=(const ObTriangle&) = default;
    ObTriangle& operator=(ObTriangle&&) = default;
    bool operator ==(const ObTriangle& t) const;

public:
    bool ContainsVertex(const ObNode& v);
    bool CircumCircleContains(const ObNode& v) const;

    bool almostEqualTriangle(const ObTriangle& ohter);
};

