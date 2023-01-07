#pragma once
class ObCircle : public GameObject, public StaticVertexCount<ObCircle>
{
private:
    static ID3D11Buffer* fillVertexBuffer;
    static ID3D11Buffer* vertexBuffer;

public:
    static void CreateStaticMember();
    static void DeleteStaticMember();

public:
    ObCircle();
    void Render()override;
};

