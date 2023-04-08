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
    //�ߺ��Ȱ� ����.
    if (iter == textureList.end())
    {
        ComPtr<ID3D11ShaderResourceView> temp;
        wstring path = L"../Contents/Images/" + file;

        ScratchImage image;
        LoadFromWICFile(path.c_str(), WIC_FLAGS_NONE, nullptr, image);

        HRESULT hr = CreateShaderResourceView(D3D.GetDevice().Get(), image.GetImages(), image.GetImageCount(),
            image.GetMetadata(), temp.GetAddressOf());


        // �ؽ��Ŀ��� ��ǥ��
        // 130, 79
        // cout << (int)image.GetPixels()[4 * (79 * 10 + 130)] << endl;
        // cout << (int)image.GetPixels()[4 * (79 * 10 + 130) + 1] << endl;
        // cout << (int)image.GetPixels()[4 * (79 * 10 + 130) + 2] << endl;

        // cout << "width: " << image.GetMetadata().width << ", ";
        // cout << "height: " << image.GetMetadata().height << endl;


        Check(hr);
        textureList[file] = temp;
        return temp;
    }
    //������ �Ҵ���ִ� �ؽ��ĸ� ��ȯ
    return iter->second;
}

bool Texture::DeleteTexture(wstring file)
{
    auto iter = textureList.find(file);

    //�ߺ��Ȱ� ����.
    if (iter == textureList.end())
        return false;

    //�ʿ����� ����
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