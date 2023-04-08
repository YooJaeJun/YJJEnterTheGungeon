#pragma once
// 선
class ObLine : public GameObject, public StaticVertexCount<ObLine>
{
private:
    static Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;

public:
    static void CreateStaticMember();
    void Render();


public:
    int     index;
    bool    isBad = false;

private:
    Vec2f v;
    Vec2f w;

public:
    ObLine();
    ObLine(const ObLine& l) = default;
    ObLine(ObLine&&) = default;
    ObLine(const Vector2& v1, const Vector2& v2);
    ObLine(const int index, const Vector2& v1, const Vector2& v2);
    ObLine(const Vec2f& v1, const Vec2f& v2);
    ObLine(const int index, const Vec2f& v1, const Vec2f& v2);
    void Init();
    void Init(const Vector2& v1, const Vector2& v2);
    void SetV(const Vec2f& other);
    void SetW(const Vec2f& other);
    void SetVIdx(const int other);
    void SetWIdx(const int other);
    const Vec2f& V() const { return v; }
    const Vec2f& W() const { return w; }

    bool operator<(const ObLine& other) const; // visited 판별용
    bool operator>(const ObLine& other) const;  // priority_queue 정렬용
    ObLine& operator=(const ObLine& other);
    ObLine& operator=(ObLine&&) = default;
    bool operator==(const ObLine & other) const;

public:
    bool NearlyEqualLine(const ObLine& other);
};


