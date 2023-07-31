#pragma once
class ObRect final : public GameObject, public StaticVertexCount<ObRect>
{
private:
    static Microsoft::WRL::ComPtr<ID3D11Buffer> fillVertexBuffer;
    static Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;

public:
    static void CreateStaticMember();

public:
    ObRect();
    void Render() override;
    Vector2 lb() const;
    Vector2 rb() const;
    Vector2 lt() const;
    Vector2 rt() const;
};
