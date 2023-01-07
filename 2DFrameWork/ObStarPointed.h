#pragma once
class ObStarPointed : public GameObject, public StaticVertexCount<ObStarPointed>
{
private:
    static ID3D11Buffer* fillVertexBuffer;
    static ID3D11Buffer* vertexBuffer;

public:
    static void CreateStaticMember();
    static void DeleteStaticMember();

public:
    ObStarPointed();
    void Render()override;
};

