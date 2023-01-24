#pragma once
class Texture
{
private:
    Texture();

public:
    static Texture& GetInstance()
    {
        static std::unique_ptr<Texture> inst;
        if (!inst)
            inst = std::unique_ptr<Texture>(new Texture());
        return *inst;
    }

private:
    std::map<wstring, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> textureList;

public:
    ~Texture();
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> LoadTexture(wstring file);
    bool DeleteTexture(wstring file);

    std::shared_ptr<ScratchImage> GetTexture(wstring file);
};

