#pragma once
class Shader
{
    Microsoft::WRL::ComPtr<ID3D11VertexShader>	vertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader>	pixelShader;
    Microsoft::WRL::ComPtr<ID3D11InputLayout>	vertexLayout;

public:
    Shader(wstring file);
    ~Shader();
    void Set();
};
