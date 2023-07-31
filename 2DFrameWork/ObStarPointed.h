#pragma once
class ObStarPointed final : public GameObject, public StaticVertexCount<ObStarPointed>
{
private:
    static Microsoft::WRL::ComPtr<ID3D11Buffer> fillVertexBuffer;
    static Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;

public:
    static void CreateStaticMember();

public:
    ObStarPointed();
    void Render()override;
};
