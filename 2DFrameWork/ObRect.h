#pragma once
class ObRect: public GameObject, public StaticVertexCount<ObRect>
{
private:
    static Microsoft::WRL::ComPtr<ID3D11Buffer> fillVertexBuffer;
    static Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;

public:
    static void CreateStaticMember();

public:
    ObRect();
    void Render() override;
    const Vector2 lb() const;
    const Vector2 rb() const;
    const Vector2 lt() const;
    const Vector2 rt() const;
};

