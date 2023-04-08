#pragma once
// »ï°¢Çü
class ObTriangle : public GameObject, public StaticVertexCount<ObTriangle>
{
private:
    static Microsoft::WRL::ComPtr<ID3D11Buffer> fillVertexBuffer;
    static Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;

public:
    static void CreateStaticMember();
    void Render();


public:
    Vec2f a;
    Vec2f b;
    Vec2f c;

public:
    bool isBad = false;

public:
    ObTriangle();
    ObTriangle(const ObTriangle&) = default;
    ObTriangle(ObTriangle&&) noexcept = default;
    ObTriangle(const Vector2& v1, const Vector2& v2, const Vector2& v3);
    ObTriangle(const Vec2f& v1, const Vec2f& v2, const Vec2f& v3);

    ObTriangle& operator=(const ObTriangle&) = default;
    ObTriangle& operator=(ObTriangle&&) = default;
    bool operator ==(const ObTriangle& t) const;

public:
    bool ContainsVertex(const Vec2f& v);
    bool CircumCircleContains(const Vec2f& v) const;

    bool almostEqualTriangle(const ObTriangle& ohter);
};

