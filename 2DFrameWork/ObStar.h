#pragma once
class ObStar final : public GameObject, public StaticVertexCount<ObStar>
{
private:
    static Microsoft::WRL::ComPtr<ID3D11Buffer> fillVertexBuffer;
    static Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;

public:
    static void CreateStaticMember();

public:
    ObStar();
    void Render()override;
};
