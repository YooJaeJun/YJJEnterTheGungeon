#pragma once
class ObCircle final :
	public GameObject,
	public StaticVertexCount<ObCircle>
{
private:
    static Microsoft::WRL::ComPtr<ID3D11Buffer> fillVertexBuffer;
    static Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;

public:
    static void CreateStaticMember();

public:
    ObCircle();
    void Render()override;
};
