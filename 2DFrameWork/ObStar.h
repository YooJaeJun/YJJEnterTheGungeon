#pragma once
class ObStar : public GameObject, public StaticVertexCount<ObStar>
{
private:
    static ID3D11Buffer* fillVertexBuffer;
    static ID3D11Buffer* vertexBuffer;

public:
    static void CreateStaticMember();
    static void DeleteStaticMember();

public:
    ObStar();
    void Render()override;
};

