#pragma once
class ObRect: public GameObject, public StaticVertexCount<ObRect>
{
private:
    static ID3D11Buffer* fillVertexBuffer;
    static ID3D11Buffer* vertexBuffer;

public:
    static void CreateStaticMember();
    static void DeleteStaticMember();

public:
    ObRect();
    void Render() override;
    const Vector2 lb() const;
    const Vector2 rb() const;
    const Vector2 lt() const;
    const Vector2 rt() const;
};

