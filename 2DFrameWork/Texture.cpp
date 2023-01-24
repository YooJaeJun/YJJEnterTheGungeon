#include "framework.h"
using Microsoft::WRL::ComPtr;

Texture::Texture()
{
}

Texture::~Texture()
{
    textureList.clear();
}

ComPtr<ID3D11ShaderResourceView> Texture::LoadTexture(wstring file)
{
    auto iter = textureList.find(file);
    //중복된게 없다.
    if (iter == textureList.end())
    {
        ComPtr<ID3D11ShaderResourceView> temp;
        wstring path = L"../Contents/Images/" + file;

        ScratchImage image;
        LoadFromWICFile(path.c_str(), WIC_FLAGS_NONE, nullptr, image);

        HRESULT hr = CreateShaderResourceView(D3D.GetDevice().Get(), image.GetImages(), image.GetImageCount(),
            image.GetMetadata(), temp.GetAddressOf());


        // 텍스쳐에서 좌표값
        // 130, 79
        // cout << (int)image.GetPixels()[4 * (79 * 10 + 130)] << endl;
        // cout << (int)image.GetPixels()[4 * (79 * 10 + 130) + 1] << endl;
        // cout << (int)image.GetPixels()[4 * (79 * 10 + 130) + 2] << endl;

        cout << "width: " << image.GetMetadata().width << ", ";
        cout << "height: " << image.GetMetadata().height << endl;


        Check(hr);
        textureList[file] = temp;
        return temp;
    }
    //기존에 할당되있던 텍스쳐를 반환
    return iter->second;
}

bool Texture::DeleteTexture(wstring file)
{
    auto iter = textureList.find(file);

    //중복된게 없다.
    if (iter == textureList.end())
    {
        return false;
    }

    //맵에서도 삭제
    textureList.erase(iter);

    return true;
}

shared_ptr<ScratchImage> Texture::GetTexture(wstring file)
{
    // ID3D11ShaderResourceView* temp;
    wstring path = L"../Contents/Images/" + file;

    std::shared_ptr<ScratchImage> image = make_shared<ScratchImage>();
    LoadFromWICFile(path.c_str(), WIC_FLAGS_NONE, nullptr, *image);

    return image;
}