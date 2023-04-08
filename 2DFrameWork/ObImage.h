#pragma once
enum class AnimState
{
    stop, //정지
    loop, //반복재생
    once, //한번만 재생
    reverseLoop, //역으로 반복재생
    reverseOnce
};

class ObImage : public GameObject, public StaticVertexCount<ObImage>
{
    friend class ObTileMap;

private:
    //static ID3D11Buffer* vertexBuffer;
    //static ID3D11Buffer* uvBuffer;
    static Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
    static Microsoft::WRL::ComPtr<ID3D11Buffer> uvBuffer;

public:
    static void CreateStaticMember();

private:
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>   SRV;
    Microsoft::WRL::ComPtr<ID3D11SamplerState>         sampler;
    D3D11_SAMPLER_DESC	        samplerDesc;

    AnimState                   animState;
    float                       animTime; //재생시킬 때 필요한 변수
    float                       animInterval; //재생간격
    bool                        animXAxis; //가로재생?

public:
    Vector4                     uv;
    Vec2i                       maxFrame;
    Vec2i                       frame;
    bool                        reverseLR;
    wstring                     file;

private:
    void PlayAnim();

public:
    ObImage(wstring file);
    ~ObImage();

    void Render()override;
    void ChangeAnim(AnimState anim, float interval, bool xAxis = true); //set함수 기능
    void ChangeSampler(D3D11_FILTER filter = D3D11_FILTER_MIN_MAG_MIP_POINT,
        D3D11_TEXTURE_ADDRESS_MODE addressU = D3D11_TEXTURE_ADDRESS_WRAP,
        D3D11_TEXTURE_ADDRESS_MODE addressV = D3D11_TEXTURE_ADDRESS_WRAP);
};

